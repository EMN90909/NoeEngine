// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#include "noe_update_manager.h"

#include "editor/editor_paths.h"
#include "editor/editor_scale.h"
#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/io/json.h"
#include "core/os/os.h"
#include "core/string/translation.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/main/http_request.h"
#include "scene/main/timer.h"

static const char *NOE_PRODUCT_VERSION = "0.1.0-dev";
static const int64_t MAX_NOE_PATCH = 104857600;

bool NoeUpdateManager::_version_newer(const String &next) const {
    String old_version = installed_version.is_empty() ? String(NOE_PRODUCT_VERSION) : installed_version;
    const bool next_dev = next.contains("-");
    const bool old_dev = old_version.contains("-");
    const String n = next.get_slice("-", 0), o = old_version.get_slice("-", 0);
    if (n.get_slice_count(".") != 3 || o.get_slice_count(".") != 3) return false;
    for (int i = 0; i < 3; ++i) {
        if (!n.get_slice(".", i).is_valid_int()) return false;
        const int64_t a = n.get_slice(".", i).to_int(), b = o.get_slice(".", i).to_int();
        if (a != b) return a > b;
    }
    return old_dev && !next_dev;
}
bool NoeUpdateManager::_valid_download_url(const String &url) {
    if (!url.begins_with("https://") || url.contains("@") || url.contains("\\") ||
            url.contains("\n") || url.contains("\r") || url.contains(" ") || url.contains("?") || url.contains("#")) return false;
    const String host = url.get_slice("/", 2);
    return host.ends_with(".supabase.co") &&
            url.contains("/storage/v1/object/public/noeengine-patches/releases/");
}
void NoeUpdateManager::_read_website() {
    website_url = OS::get_singleton()->get_environment("NOEENGINE_AUTH_URL").strip_edges();
    if (website_url.is_empty() && EditorPaths::get_singleton() && EditorPaths::get_singleton()->are_paths_valid()) {
        Ref<ConfigFile> cfg;
        cfg.instantiate();
        if (cfg->load(EditorPaths::get_singleton()->get_config_dir().path_join("noeengine_account.cfg")) == OK)
            website_url = String(cfg->get_value("account", "website_url", ""));
    }
    if (!website_url.begins_with("https://") || website_url.contains("@") ||
            website_url.contains("?") || website_url.contains("#") ||
            website_url.contains("\n") || website_url.contains(" ")) {
        website_url.clear();
    }
    while (website_url.ends_with("/")) website_url = website_url.substr(0, website_url.length() - 1);
}
NoeUpdateManager::NoeUpdateManager() {
    metadata_request = memnew(HTTPRequest);
    metadata_request->set_timeout(12);
    metadata_request->set_max_redirects(0);
    metadata_request->set_body_size_limit(32768);
    metadata_request->connect("request_completed", callable_mp(this, &NoeUpdateManager::_on_manifest));
    add_child(metadata_request);
    patch_request = memnew(HTTPRequest);
    patch_request->set_timeout(180);
    patch_request->set_max_redirects(0);
    patch_request->set_body_size_limit(MAX_NOE_PATCH);
    patch_request->connect("request_completed", callable_mp(this, &NoeUpdateManager::_on_patch));
    add_child(patch_request);
    release_dialog = memnew(ConfirmationDialog);
    release_dialog->set_title(TTR("NoeEngine update available"));
    release_dialog->set_ok_button_text(TTR("Download update"));
    release_dialog->set_cancel_button_text(TTR("Later"));
    release_dialog->connect("confirmed", callable_mp(this, &NoeUpdateManager::_download));
    add_child(release_dialog);
    completion_dialog = memnew(AcceptDialog);
    completion_dialog->set_title(TTR("NoeEngine update"));
    completion_dialog->get_ok_button()->set_text(TTR("Continue"));
    add_child(completion_dialog);
    install_dialog = memnew(ConfirmationDialog);
    install_dialog->set_title(TTR("Install signed update"));
    install_dialog->set_ok_button_text(TTR("Install and restart"));
    install_dialog->set_cancel_button_text(TTR("Not now"));
    install_dialog->connect("confirmed", callable_mp(this, &NoeUpdateManager::_install_and_restart));
    add_child(install_dialog);
    auto_timer = memnew(Timer);
    auto_timer->set_wait_time(300.0);
    auto_timer->connect("timeout", callable_mp(this, &NoeUpdateManager::_startup_check));
    add_child(auto_timer);
    startup_timer = memnew(Timer);
    startup_timer->set_one_shot(true);
    startup_timer->set_wait_time(8);
    startup_timer->connect("timeout", callable_mp(this, &NoeUpdateManager::_startup_check));
    add_child(startup_timer);
}
NoeUpdateManager *NoeUpdateManager::install(Control *parent, HBoxContainer *toolbar) {
    NoeUpdateManager *manager = memnew(NoeUpdateManager);
    parent->add_child(manager);
    Button *b = memnew(Button);
    b->set_flat(true);
    b->set_text(TTR("Updates"));
    b->set_tooltip_text(TTR("Check for updates from your configured NoeEngine website"));
    b->set_focus_mode(Control::FOCUS_ALL);
    b->connect("pressed", callable_mp(manager, &NoeUpdateManager::_manual_check));
    toolbar->add_child(b);
    manager->update_button = b;
    return manager;
}
void NoeUpdateManager::_notification(int what) {
    if (what == NOTIFICATION_READY) {
        startup_timer->start();
        auto_timer->start();
        _show_post_restart_notice();
    }
}
void NoeUpdateManager::_show_notice(const String &title, const String &message) {
    completion_dialog->set_title(title);
    completion_dialog->set_text(message);
    completion_dialog->popup_centered(Size2i(560, 270) * EDSCALE);
}
void NoeUpdateManager::_manual_check() { quiet_check = false; _check(); }
void NoeUpdateManager::_startup_check() { quiet_check = true; _check(); }
void NoeUpdateManager::_check() {
    if (downloading || update_button->is_disabled()) return;
    _read_website();
    if (website_url.is_empty()) {
        _show_notice(TTR("Configure update source"),
                TTR("First set your trusted HTTPS NoeEngine website in Account > Security."));
        return;
    }
    const String path = website_url + "/updates/latest?platform=" +
#if defined(WINDOWS_ENABLED)
        String("windows");
#elif defined(MACOS_ENABLED)
        String("macos");
#else
        String("linuxbsd");
#endif
    const Error status = metadata_request->request(path + "&channel=stable");
    if (status == OK) { update_button->set_text(TTR("Checking...")); update_button->set_disabled(true); }
    else _show_notice(TTR("Update check failed"), TTR("The update server is busy. Try again."));
}
void NoeUpdateManager::_on_manifest(int result, int status, const PackedStringArray &headers, const PackedByteArray &body) {
    update_button->set_text(TTR("Updates"));
    update_button->set_disabled(false);
    if (status == 204) return; // No releases published for this platform.
    if (result != HTTPRequest::RESULT_SUCCESS || status != 200) {
        if (quiet_check) return;
        _show_notice(TTR("Update check failed"), TTR("The NoeEngine release catalog is unavailable."));
        return;
    }
    const Variant parsed = JSON::parse_string(body.get_string_from_utf8());
    if (parsed.get_type() != Variant::DICTIONARY) {
        _show_notice(TTR("Invalid release"), TTR("The server returned an invalid release manifest.")); return;
    }
    Dictionary info = parsed;
    const String version = info.get("version", "");
    const String hash = info.get("sha256", "");
    const String url = info.get("download_url", "");
    const String filename = info.get("filename", "");
    const String notes = info.get("release_notes", "");
    const int64_t expected_size = (int64_t)info.get("size_bytes", (int64_t)0);
    if (!_version_newer(version)) return;
    if (hash.length() != 64 || !_valid_download_url(url) || filename.is_empty() ||
            filename.contains("/") || filename.contains("\\") || filename.contains("..") ||
            expected_size <= 0 || expected_size > MAX_NOE_PATCH || notes.length() > 10000) {
        _show_notice(TTR("Invalid release"), TTR("The update manifest failed validation.")); return;
    }
    for (int i = 0; i < 64; ++i) {
        const char32_t c = hash[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return;
    }
    // No silent package installation: an OS-level installer must confirm executable changes.
    available = true;
    release_version = version;
    patch_sha256 = hash;
    patch_filename = filename;
    patch_size = expected_size;
    release_notes = notes;
    download_url = url;
    update_button->set_text(TTR("Update available"));
    release_dialog->set_text(vformat(TTR("NoeEngine %s is available.\n\nWhat's new:\n%s\n\nDownload and verify the patch? Installation requires your system package manager."), version, notes));
    release_dialog->popup_centered(Size2i(580, 370) * EDSCALE);
}
void NoeUpdateManager::_download() {
    if (!available || downloading || !EditorPaths::get_singleton()) return;
    const String directory = EditorPaths::get_singleton()->get_cache_dir().path_join("noeengine-updates");
    if (DirAccess::make_dir_recursive_absolute(directory) != OK) {
        _show_notice(TTR("Update error"), TTR("Cannot create a staging directory.")); return;
    }
    saved_path = directory.path_join(release_version + "-" + patch_filename);
    // Download exclusively to the dedicated cache folder, not the engine installation.
    patch_request->set_download_file(saved_path);
    const Error result = patch_request->request(download_url);
    if (result != OK) {
        _show_notice(TTR("Download error"), TTR("The download could not start.")); return;
    }
    downloading = true;
    update_button->set_disabled(true);
    update_button->set_text(TTR("Downloading..."));
}
void NoeUpdateManager::_on_patch(int result, int status, const PackedStringArray &headers, const PackedByteArray &body) {
    downloading = false;
    update_button->set_disabled(false);
    update_button->set_text(available ? TTR("Update available") : TTR("Updates"));
    if (result != HTTPRequest::RESULT_SUCCESS || status != 200 ||
            !FileAccess::exists(saved_path)) {
        if (!saved_path.is_empty()) DirAccess::remove_absolute(saved_path);
        _show_notice(TTR("Download failed"), TTR("Could not safely stage the update.")); return;
    }
    Ref<FileAccess> file = FileAccess::open(saved_path, FileAccess::READ);
    const bool valid_size = file.is_valid() && (int64_t)file->get_length() == patch_size;
    file.unref();
    const bool valid_hash = valid_size && FileAccess::get_sha256(saved_path).to_lower() == patch_sha256;
    if (!valid_hash) {
        DirAccess::remove_absolute(saved_path);
        _show_notice(TTR("Verification failed"), TTR("The package checksum or size did not match. The download was discarded.")); return;
    }
    trusted_helper = OS::get_singleton()->get_environment("NOEENGINE_PATCH_HELPER").strip_edges();
    signing_public_key = OS::get_singleton()->get_environment("NOEENGINE_UPDATE_PUBKEY").strip_edges();
    const bool supported = patch_filename.ends_with(".noepatch") && trusted_helper.is_absolute_path() &&
            FileAccess::exists(trusted_helper) && signing_public_key.length() == 44;
    if (!supported) {
        _show_notice(TTR("Update downloaded and verified"),
                vformat(TTR("NoeEngine %s downloaded.\n\nWhat's new:\n%s\n\nSaved at: %s\n\nAutomatic installation requires a trusted updater and Ed25519 public key."),
                        release_version, release_notes, saved_path));
        return;
    }
    install_dialog->set_text(vformat(TTR("Signed patch %s is downloaded.\n\nWhat's new:\n%s\n\nThe editor closes while the trusted updater verifies and changes files. No elevated privileges are requested. Continue?"),
            release_version, release_notes));
    install_dialog->popup_centered(Size2i(590, 360) * EDSCALE);
}
