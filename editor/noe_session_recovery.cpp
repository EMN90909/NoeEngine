#include "noe_session_recovery.h"

#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/string/print_string.h"
#include "editor/editor_paths.h"

#if defined(WINDOWS_ENABLED)
// SDK macros stay isolated in this platform implementation, never a shared header.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace NoeSessionRecovery {
static String marker_path() {
    return EditorPaths::get_singleton()->get_project_settings_dir().path_join("noecore-session.active");
}

Error atomic_replace(const String &temporary, const String &target) {
#if defined(WINDOWS_ENABLED)
    const String source = ProjectSettings::get_singleton()->globalize_path(temporary);
    const String destination = ProjectSettings::get_singleton()->globalize_path(target);
    // DirAccessWindows::rename removes the old file first. MoveFileExW replaces
    // it without explicitly deleting it and preserves the old file on failure.
    return MoveFileExW(reinterpret_cast<LPCWSTR>(source.utf16().get_data()),
                      reinterpret_cast<LPCWSTR>(destination.utf16().get_data()),
                      MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) ? OK : FAILED;
#else
    // Same-directory POSIX rename atomically replaces the previous file.
    return DirAccess::rename_absolute(temporary, target);
#endif
}

Error atomic_save_layout(Ref<ConfigFile> layout) {
    const String target = EditorPaths::get_singleton()->get_project_settings_dir().path_join("editor_layout.cfg");
    const String temporary = target + ".noecore-tmp";
    const Error saved = layout->save(temporary);
    if (saved != OK) {
        DirAccess::remove_absolute(temporary);
        return saved;
    }
    const Error replaced = atomic_replace(temporary, target);
    if (replaced != OK) {
        DirAccess::remove_absolute(temporary);
    }
    return replaced;
}

void begin() {
    const String marker = marker_path();
    if (FileAccess::exists(marker)) {
        // The native editor layout contains saved open-scene tabs. Neither the
        // crash marker nor the diagnostic message contains private paths.
        WARN_PRINT("NoeCore: Previous editor session ended unexpectedly. Saved tabs may be restored from the editor layout; unsaved edits may be lost.");
    }
    const String temporary = marker + ".tmp";
    Ref<FileAccess> file = FileAccess::open(temporary, FileAccess::WRITE);
    if (file.is_null()) {
        WARN_PRINT("NoeCore: Cannot create local session recovery marker.");
        return;
    }
    file->store_string("active\n");
    file->flush();
    file.unref();
    if (atomic_replace(temporary, marker) != OK) {
        DirAccess::remove_absolute(temporary);
        WARN_PRINT("NoeCore: Cannot commit local session recovery marker.");
    }
}

void finish() {
    DirAccess::remove_absolute(marker_path());
}
} // namespace NoeSessionRecovery
