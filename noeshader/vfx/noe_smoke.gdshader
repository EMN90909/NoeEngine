shader_type spatial;
render_mode blend_mix, depth_draw_never, cull_disabled, unshaded, depth_prepass_alpha;
uniform sampler2D smoke_texture : source_color, filter_linear_mipmap;
uniform sampler2D noise_texture : repeat_enable, filter_linear_mipmap;
uniform vec4 smoke_tint : source_color = vec4(0.38, 0.40, 0.43, 0.55);
uniform float noise_scale : hint_range(0.1, 10.0) = 2.0;
uniform vec2 wind = vec2(0.03, 0.06);
uniform float edge_softness : hint_range(0.01, 0.5) = 0.15;
void fragment() {
    vec2 moving_uv = UV + wind * TIME;
    float noise = texture(noise_texture, moving_uv * noise_scale).r;
    vec4 smoke = texture(smoke_texture, UV);
    float radial = 1.0 - smoothstep(0.5 - edge_softness, 0.5, length(UV - vec2(0.5)));
    ALBEDO = smoke_tint.rgb * mix(0.75, 1.2, noise);
    ALPHA = smoke.a * smoke_tint.a * radial * COLOR.a;
}
