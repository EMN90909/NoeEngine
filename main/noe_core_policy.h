// NoeCore policy primitives. No engine types or allocation on the hot path.
#ifndef NOE_CORE_POLICY_H
#define NOE_CORE_POLICY_H

#include <cstdint>
#include <limits>

namespace NoeCore {
constexpr uint64_t GiB = 1024ULL * 1024 * 1024;

enum class Profile { LOW, BALANCED, HIGH, ULTRA };
struct Hardware {
    uint32_t cpu_threads = 0;
    uint64_t ram_bytes = 0;
    uint64_t vram_bytes = 0; // 0 means unknown, NOT no VRAM.
    bool hardware_gpu = false;
    bool graphics_api_supported = false;
};

inline Profile recommend(const Hardware &h) {
    // An unverified renderer is not evidence for selecting a costly preset.
    if (!h.hardware_gpu || !h.graphics_api_supported) return Profile::LOW;
    if (h.cpu_threads && h.cpu_threads < 4) return Profile::LOW;
    if (h.ram_bytes && h.ram_bytes < 6 * GiB) return Profile::LOW;
    if (!h.ram_bytes || !h.cpu_threads || !h.vram_bytes) return Profile::BALANCED;
    if (h.cpu_threads >= 12 && h.ram_bytes >= 24 * GiB && h.vram_bytes >= 8 * GiB) return Profile::ULTRA;
    if (h.cpu_threads >= 8 && h.ram_bytes >= 12 * GiB && h.vram_bytes >= 4 * GiB) return Profile::HIGH;
    return h.vram_bytes < 2 * GiB ? Profile::LOW : Profile::BALANCED;
}
inline const char *profile_name(Profile p) {
    switch (p) {
        case Profile::LOW: return "Low";
        case Profile::BALANCED: return "Balanced";
        case Profile::HIGH: return "High";
        case Profile::ULTRA: return "Ultra";
    }
    return "Low";
}

struct CacheEntry {
    uint64_t size_bytes = 0;
    uint64_t last_used_frame = 0;
    uint32_t external_references = 0;
    bool pinned = false;
    bool in_active_scene = false;
    bool loading = false;
};

class MemoryBudget {
    uint64_t limit_bytes = 0;
    uint64_t used_bytes = 0;
public:
    explicit MemoryBudget(uint64_t limit = 0) : limit_bytes(limit) {}
    void set_limit(uint64_t limit) { limit_bytes = limit; }
    uint64_t limit() const { return limit_bytes; }
    uint64_t usage() const { return used_bytes; }
    void account(uint64_t bytes) {
        used_bytes = bytes > std::numeric_limits<uint64_t>::max() - used_bytes ?
                std::numeric_limits<uint64_t>::max() : used_bytes + bytes;
    }
    void release(uint64_t bytes) { used_bytes = bytes > used_bytes ? 0 : used_bytes - bytes; }
    bool over_budget() const { return limit_bytes && used_bytes > limit_bytes; }
    bool may_evict(const CacheEntry &item, uint64_t now_frame, uint64_t grace_frames = 120) const {
        return over_budget() && item.size_bytes != 0 && !item.pinned &&
                !item.in_active_scene && !item.loading && !item.external_references &&
                now_frame >= item.last_used_frame && now_frame - item.last_used_frame >= grace_frames;
    }
};
} // namespace NoeCore
#endif // NOE_CORE_POLICY_H
