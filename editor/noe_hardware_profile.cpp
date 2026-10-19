// Platform-isolated NoeEngine editor hardware memory probe.
// OS SDK headers must never be included from a shared Godot header.
#include "noe_hardware_profile.h"

#include <cstdio>
#if defined(WINDOWS_ENABLED)
#include <windows.h>
#elif defined(MACOS_ENABLED)
#include <sys/sysctl.h>
#endif

namespace NoeHardwareProfile {
uint64_t installed_memory_bytes() {
#if defined(WINDOWS_ENABLED)
    MEMORYSTATUSEX state = {};
    state.dwLength = sizeof(state);
    return GlobalMemoryStatusEx(&state) ? static_cast<uint64_t>(state.ullTotalPhys) : 0;
#elif defined(MACOS_ENABLED)
    uint64_t size = 0;
    size_t length = sizeof(size);
    return sysctlbyname("hw.memsize", &size, &length, nullptr, 0) == 0 ? size : 0;
#elif defined(LINUXBSD_ENABLED)
    // /proc/meminfo is Linux-specific; BSD safely chooses conservative defaults.
    FILE *file = std::fopen("/proc/meminfo", "r");
    if (!file) return 0;
    unsigned long long kib = 0;
    const int found = std::fscanf(file, "MemTotal: %llu kB", &kib);
    std::fclose(file);
    return found == 1 ? static_cast<uint64_t>(kib) * 1024ULL : 0;
#else
    return 0;
#endif
}

bool use_classic_ui(uint64_t bytes) {
    return bytes == 0 || bytes < 4ULL * 1024 * 1024 * 1024;
}

bool use_classic_ui() {
    return use_classic_ui(installed_memory_bytes());
}

bool prefer_compatibility_renderer(uint64_t bytes) {
    return bytes == 0 || bytes < 8ULL * 1024 * 1024 * 1024;
}

bool prefer_compatibility_renderer() {
    return prefer_compatibility_renderer(installed_memory_bytes());
}
} // namespace NoeHardwareProfile
