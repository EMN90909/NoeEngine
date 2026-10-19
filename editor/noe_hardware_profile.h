// Lightweight NoeEngine hardware presentation and new-project graphics defaults.
// The platform API implementations live in .cpp to prevent Windows macros
// leaking into unrelated editor and rendering headers.
#ifndef NOE_HARDWARE_PROFILE_H
#define NOE_HARDWARE_PROFILE_H

#include <cstdint>
namespace NoeHardwareProfile {
uint64_t installed_memory_bytes();
bool use_classic_ui(uint64_t installed_bytes);
bool use_classic_ui();
bool prefer_compatibility_renderer(uint64_t installed_bytes);
bool prefer_compatibility_renderer();
} // namespace NoeHardwareProfile
#endif // NOE_HARDWARE_PROFILE_H
