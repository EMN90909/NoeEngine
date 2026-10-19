// NoeCore local crash-session marker and atomic editor layout persistence.
// Unsaved in-memory scene edits are NOT reconstructed by this marker.
#ifndef NOE_SESSION_RECOVERY_H
#define NOE_SESSION_RECOVERY_H

#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/string/print_string.h"
#include "editor/editor_paths.h"

namespace NoeSessionRecovery {
inline String marker_path() {
    return EditorPaths::get_singleton()->get_project_settings_dir().path_join("noecore-session.active");
}
inline Error atomic_save_layout(const Ref<ConfigFile> &layout) {
    const String target = EditorPaths::get_singleton()->get_project_settings_dir().path_join("editor_layout.cfg");
    const String temporary = target + ".noecore-tmp";
    const Error saved = layout->save(temporary);
    if (saved != OK) {
        DirAccess::remove_absolute(temporary);
        return saved;
    }
    const Error renamed = DirAccess::rename_absolute(temporary, target);
    if (renamed != OK) {
        DirAccess::remove_absolute(temporary);
    }
    return renamed;
}
inline void begin() {
    const String marker = marker_path();
    if (FileAccess::exists(marker)) {
        // Existing native scene layout still contains the last saved open tabs.
        // Do not print paths or copy project data into a diagnostic report.
        WARN_PRINT("NoeCore: Previous editor session ended unexpectedly. Saved tabs can be restored from the editor layout; unsaved changes may be lost.");
    }
    const String temporary = marker + ".tmp";
    Ref<FileAccess> file = FileAccess::open(temporary, FileAccess::WRITE);
    if (file.is_null()) {
        WARN_PRINT("NoeCore: Unable to create a local session recovery marker.");
        return;
    }
    file->store_string("active\n");
    file->flush();
    file.unref();
    if (DirAccess::rename_absolute(temporary, marker) != OK) {
        DirAccess::remove_absolute(temporary);
        WARN_PRINT("NoeCore: Unable to commit local session recovery marker.");
    }
}
inline void finish() {
    DirAccess::remove_absolute(marker_path());
}
} // namespace NoeSessionRecovery
#endif // NOE_SESSION_RECOVERY_H
