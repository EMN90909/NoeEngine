// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOE_UPDATE_MANAGER_H
#define NOE_UPDATE_MANAGER_H

#include "scene/main/node.h"
#include "core/variant/variant.h"

class HBoxContainer;
class Control;
class Button;
class HTTPRequest;
class ConfirmationDialog;
class AcceptDialog;
class Timer;

// Downloads only allowlisted HTTPS release packages to a private staging folder.
// Never executes patch contents or elevates privileges. OS installation remains manual.
class NoeUpdateManager : public Node {
    GDCLASS(NoeUpdateManager, Node);
    Button *update_button = nullptr;
    HTTPRequest *metadata_request = nullptr;
    HTTPRequest *patch_request = nullptr;
    ConfirmationDialog *release_dialog = nullptr;
    AcceptDialog *completion_dialog = nullptr;
    Timer *startup_timer = nullptr;
    String website_url;
    String download_url;
    String patch_sha256;
    String patch_filename;
    String release_version;
    String release_notes;
    String saved_path;
    String installed_version;
    String trusted_helper;
    String signing_public_key;
    int64_t patch_size = 0;
    bool available = false;
    bool downloading = false;
    bool quiet_check = false;

    static void _bind_methods() {}
    void _read_website();
    void _check();
    void _manual_check();
    void _startup_check();
    void _on_manifest(int p_result, int p_status, const PackedStringArray &p_headers, const PackedByteArray &p_body);
    void _download();
    void _install_and_restart();
    void _show_post_restart_notice();
    void _on_patch(int p_result, int p_status, const PackedStringArray &p_headers, const PackedByteArray &p_body);
    void _show_notice(const String &p_title, const String &p_message);
    bool _version_newer(const String &p_version) const;
    static bool _valid_download_url(const String &p_url);

protected:
    void _notification(int p_what);

public:
    static NoeUpdateManager *install(Control *p_parent, HBoxContainer *p_toolbar);
    NoeUpdateManager();
};

#endif
