// NoeRender game quality adapter. Does not rename legacy renderer/driver IDs.
#ifndef NOE_RENDER_QUALITY_H
#define NOE_RENDER_QUALITY_H
#include "core/config/project_settings.h"
#include "core/string/print_string.h"
#include <cstdint>
#include <cstdio>
#include "main/noe_render_quality_policy.h"
namespace NoeRender {
// Only Linux procfs can be queried here without platform header macro leaks.
// Unknown RAM is reported as 0: a heuristic must not masquerade as GPU probing.
inline uint64_t physical_ram_bytes() {
#if defined(LINUXBSD_ENABLED)
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
inline Tier parse(const String &name) {
    const String n = name.strip_edges().to_lower();
    if (n == "lite") return Tier::LITE;
    if (n == "balanced") return Tier::BALANCED;
    if (n == "high") return Tier::HIGH;
    if (n == "ultra") return Tier::ULTRA;
    if (n == "custom") return Tier::CUSTOM;
    if (n != "auto") WARN_PRINT("NoeRender: Unknown preset, using Auto.");
    return Tier::AUTO;
}
inline void configure(ProjectSettings *settings, const String &renderer, bool editor_or_project_manager) {
    const String migrated = settings->has_setting("rendering/noe_plus/quality/preset") ? String(settings->get_setting("rendering/noe_plus/quality/preset")) : "Auto";
    const String selected = GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING,
            "rendering/noe_render/quality/preset", PROPERTY_HINT_ENUM,
            "Auto,Lite,Balanced,High,Ultra,Custom"), migrated);
    const bool adaptive_requested = GLOBAL_DEF_BASIC("rendering/noe_render/quality/adaptive_3d_scaling", true);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::FLOAT, "rendering/noe_render/quality/target_frame_ms",
            PROPERTY_HINT_RANGE, "8,50,0.5"), 16.667);
    if (editor_or_project_manager) return; // Never downscale editor controls.
    const Tier requested = parse(selected);
    if (requested == Tier::CUSTOM) return; // Honor hand-tuned project settings.
    const bool compatibility = renderer != "forward_plus"; // Mobile and GL lack Forward+ FSR 1.0.
    const Tier effective = resolve(requested, compatibility, physical_ram_bytes());
    const Preset p = preset(effective, compatibility);
    if (compatibility && requested != Tier::LITE && requested != Tier::AUTO)
        WARN_PRINT("NoeRender: Forward+-only quality is unavailable in Compatibility; applying Lite.");
    settings->set_setting("rendering/anti_aliasing/quality/msaa_3d", p.msaa_3d);
    settings->set_setting("rendering/lights_and_shadows/positional_shadow/atlas_size", p.shadow_atlas);
    settings->set_setting("rendering/mesh_lod/lod_change/threshold_pixels", p.lod_threshold);
    settings->set_setting("rendering/scaling_3d/mode", p.scaling_mode);
    settings->set_setting("rendering/scaling_3d/scale", p.scale);
    settings->set_setting("rendering/noe_render/quality/adaptive_active",
            adaptive_requested && !compatibility);
}
} // namespace NoeRender
#endif
