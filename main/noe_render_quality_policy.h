// Native Noe+ rendering quality policy, independent of GPU drivers and Godot.
#ifndef NOE_RENDER_QUALITY_POLICY_H
#define NOE_RENDER_QUALITY_POLICY_H
#include <cmath>
#include <cstdint>
namespace NoePlus {
enum class Tier { AUTO, LITE, BALANCED, HIGH, ULTRA, CUSTOM };
struct Preset { float scale; int scaling_mode; int msaa_3d; int shadow_atlas; float lod_threshold; };
inline Tier resolve(Tier requested, bool compatibility, uint64_t physical_ram_bytes) {
    if (requested == Tier::CUSTOM) return requested;
    if (compatibility) return Tier::LITE; // No Vulkan-only effects on GL.
    if (requested != Tier::AUTO) return requested;
    const uint64_t low_ram = 8ULL * 1024 * 1024 * 1024;
    return physical_ram_bytes && physical_ram_bytes < low_ram ? Tier::LITE : Tier::BALANCED;
}
inline Preset preset(Tier level, bool compatibility) {
    if (compatibility) return { 1.0f, 0, 0, 1024, 2.0f };
    switch (level) {
        case Tier::LITE: return { 0.75f, 1, 0, 1024, 2.0f };
        case Tier::BALANCED: return { 0.90f, 1, 1, 2048, 1.5f };
        case Tier::HIGH: return { 1.0f, 0, 2, 4096, 1.0f };
        case Tier::ULTRA: return { 1.0f, 0, 3, 8192, 1.0f };
        default: return { 1.0f, 0, 0, 4096, 1.0f };
    }
}
// Conservative CPU-frame proxy (not GPU timings): adjusts 3D viewport scaling
// in 0.05 steps, 45-frame observation windows, hysteresis and cooldown.
class AdaptiveScale {
    float scale = 1.0f, minimum = 0.65f, maximum = 1.0f;
    double smoothed_ms = 0.0, target_ms = 16.667;
    int samples = 0, cooldown = 0, slow_windows = 0, fast_windows = 0;
public:
    void reset(float initial, float lower, float upper, double frame_budget_ms) {
        minimum = lower >= 0.5f && lower <= 1.0f ? lower : 0.65f;
        maximum = upper >= minimum && upper <= 1.0f ? upper : 1.0f;
        scale = initial < minimum ? minimum : (initial > maximum ? maximum : initial);
        target_ms = frame_budget_ms >= 8.0 && frame_budget_ms <= 50.0 ? frame_budget_ms : 16.667;
        smoothed_ms = 0.0;
        samples = cooldown = slow_windows = fast_windows = 0;
    }
    float current_scale() const { return scale; }
    bool sample(double frame_ms, float *new_scale) {
        if (!new_scale || !std::isfinite(frame_ms) || frame_ms <= 0.0 || frame_ms > 250.0) return false;
        smoothed_ms = smoothed_ms == 0.0 ? frame_ms : 0.95 * smoothed_ms + 0.05 * frame_ms;
        if (++samples < 45) return false;
        samples = 0;
        if (cooldown > 0) { --cooldown; return false; }
        slow_windows = smoothed_ms > target_ms * 1.15 ? slow_windows + 1 : 0;
        fast_windows = smoothed_ms < target_ms * 0.75 ? fast_windows + 1 : 0;
        if (slow_windows < 2 && fast_windows < 3) return false;
        float proposed = scale;
        if (slow_windows >= 2 && scale > minimum) {
            proposed = scale - 0.05f;
            if (proposed < minimum) proposed = minimum;
        } else if (fast_windows >= 3 && scale < maximum) {
            proposed = scale + 0.05f;
            if (proposed > maximum) proposed = maximum;
        }
        slow_windows = fast_windows = 0;
        if (std::fabs(proposed - scale) < 0.001f) return false;
        scale = proposed;
        cooldown = 3;
        *new_scale = scale;
        return true;
    }
};
} // namespace NoePlus
#endif
