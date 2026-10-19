// NoeCore local crash-session marker and atomic editor layout persistence.
// Unsaved in-memory scene edits are NOT reconstructed by this marker.
#ifndef NOE_SESSION_RECOVERY_H
#define NOE_SESSION_RECOVERY_H

#include "core/io/config_file.h"

namespace NoeSessionRecovery {
// Must be implemented per OS: Windows DirAccess::rename() deletes destinations!
Error atomic_replace(const String &temporary, const String &target);
Error atomic_save_layout(const Ref<ConfigFile> &layout);
void begin();
void finish();
} // namespace NoeSessionRecovery
#endif // NOE_SESSION_RECOVERY_H
