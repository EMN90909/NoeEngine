// Lightweight NoeEngine editor presentation profile. Platform APIs are used
// only once when editor defaults are initialized; no runtime polling.
#ifndef NOE_HARDWARE_PROFILE_H
#define NOE_HARDWARE_PROFILE_H
#include <cstdint>
#include <cstdio>
#if defined(WINDOWS_ENABLED)
#include <windows.h>
#elif defined(MACOS_ENABLED)
#include <sys/sysctl.h>
#endif

namespace NoeHardwareProfile {
static inline uint64_t installed_memory_bytes() {
#if defined(WINDOWS_ENABLED)
    MEMORYSTATUSEX state = {};
    state.dwLength = sizeof(state);
    return GlobalMemoryStatusEx(&state) ? static_cast<uint64_t>(state.ullTotalPhys) : 0;
#elif defined(MACOS_ENABLED)
    uint64_t size = 0;
    size_t length = sizeof(size);
    return sysctlbyname("hw.memsize", &size, &length, nullptr, 0) == 0 ? size : 0;
#elif defined(LINUXBSD_ENABLED)
    // /proc/meminfo is Linux-specific; other BSD hosts safely use classic UI.
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return 0;
    unsigned long long kib = 0;
    const int found = fscanf(file, "MemTotal: %llu kB", &kib);
    fclose(file);
    return found == 1 ? static_cast<uint64_t>(kib) * 1024ULL : 0;
#else
    // Prefer the inexpensive classic preset where total RAM cannot be verified.
    return 0;
#endif
}
static inline bool use_classic_ui(uint64_t installed_bytes) {
    // Strict threshold: Systems with less than 4 GiB select the lightweight classic theme.
    return installed_bytes == 0 || installed_bytes < 4ULL * 1024 * 1024 * 1024;
}
static inline bool use_classic_ui() { return use_classic_ui(installed_memory_bytes()); }
// New-project rendering recommendation, separate from classic UI threshold.
// RAM is a heuristic, never a substitute for real Vulkan feature probing.
static inline bool prefer_compatibility_renderer(uint64_t bytes) {
    return bytes == 0 || bytes < 8ULL * 1024 * 1024 * 1024;
}
static inline bool prefer_compatibility_renderer() {
    return prefer_compatibility_renderer(installed_memory_bytes());
}
} // namespace NoeHardwareProfile
#endif // NOE_HARDWARE_PROFILE_H
