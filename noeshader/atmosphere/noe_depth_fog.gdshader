shader_type spatial;
render_mode unshaded, fog_disabled, cull_disabled, depth_draw_never;
// Full-screen QuadMesh material; requires depth texture and correct camera projection.
uniform sampler2D depth_texture : hint_depth_texture, filter_nearest;
uniform vec3 fog_color : source_color = vec3(0.47, 0.58, 0.67);
uniform float density : hint_range(0.0, 0.2) = 0.025;
uniform float start_distance : hint_range(0.0, 1000.0) = 10.0;
uniform float height_falloff : hint_range(0.0, 2.0) = 0.04;
void vertex() { POSITION = vec4(VERTEX.xy, 1.0, 1.0); }
void fragment() {
    float depth = texture(depth_texture, SCREEN_UV).r;
    vec3 ndc = vec3(SCREEN_UV * 2.0 - 1.0, depth);
#if CURRENT_RENDERER == RENDERER_COMPATIBILITY
    ndc.z = depth * 2.0 - 1.0;
#endif
    vec4 view = INV_PROJECTION_MATRIX * vec4(ndc, 1.0);
    view.xyz /= max(abs(view.w), 0.00001);
    float dist = max(length(view.xyz) - start_distance, 0.0);
    float fog = 1.0 - exp(-dist * density);
    fog *= exp(-max((INV_VIEW_MATRIX * vec4(view.xyz, 1.0)).y, 0.0) * height_falloff);
    ALBEDO = fog_color;
    ALPHA = clamp(fog, 0.0, 1.0);
}
