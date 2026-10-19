// Noe+ game quality adapter. Does not rename legacy renderer/driver IDs.
#ifndef NOE_RENDER_QUALITY_H
#define NOE_RENDER_QUALITY_H
#include "core/config/project_settings.h"
#include "core/string/print_string.h"
#include "editor/noe_hardware_profile.h"
#include "main/noe_render_quality_policy.h"
namespace NoePlus {
inline Tier parse(const String &name) {
    const String n = name.strip_edges().to_lower();
    if (n == "lite") return Tier::LITE;
    if (n == "balanced") return Tier::BALANCED;
    if (n == "high") return Tier::HIGH;
    if (n == "ultra") return Tier::ULTRA;
    if (n == "custom") return Tier::CUSTOM;
    if (n != "auto") WARN_PRINT("Noe+: Unknown preset, using Auto.");
    return Tier::AUTO;
}
inline void configure(ProjectSettings *settings, const String &renderer, bool editor_or_project_manager) {
    const String selected = GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING,
            "rendering/noe_plus/quality/preset", PROPERTY_HINT_ENUM,
            "Auto,Lite,Balanced,High,Ultra,Custom"), "Auto");
    const bool adaptive_requested = GLOBAL_DEF_BASIC("rendering/noe_plus/quality/adaptive_3d_scaling", true);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::FLOAT, "rendering/noe_plus/quality/target_frame_ms",
            PROPERTY_HINT_RANGE, "8,50,0.5"), 16.667);
    if (editor_or_project_manager) return; // Never downscale editor controls.
    const Tier requested = parse(selected);
    if (requested == Tier::CUSTOM) return; // Honor hand-tuned project settings.
    const bool compatibility = renderer == "gl_compatibility";
    const Tier effective = resolve(requested, compatibility, NoeHardwareProfile::installed_memory_bytes());
    const Preset p = preset(effective, compatibility);
    if (compatibility && requested != Tier::LITE && requested != Tier::AUTO)
        WARN_PRINT("Noe+: Forward+-only quality is unavailable in Compatibility; applying Lite.");
    settings->set_setting("rendering/anti_aliasing/quality/msaa_3d", p.msaa_3d);
    settings->set_setting("rendering/lights_and_shadows/positional_shadow/atlas_size", p.shadow_atlas);
    settings->set_setting("rendering/mesh_lod/lod_change/threshold_pixels", p.lod_threshold);
    settings->set_setting("rendering/scaling_3d/mode", p.scaling_mode);
    settings->set_setting("rendering/scaling_3d/scale", p.scale);
    settings->set_setting("rendering/noe_plus/quality/adaptive_active",
            adaptive_requested && !compatibility);
}
} // namespace NoePlus
#endif
