shader_type spatial;
render_mode blend_mix, depth_draw_opaque, cull_back, diffuse_burley, specular_schlick_ggx;
// NoeShader 3DCore — tune uniforms from ShaderMaterial without editing source.
group_uniforms Surface;
uniform vec4 base_color : source_color = vec4(1.0);
uniform sampler2D albedo_map : source_color, filter_linear_mipmap, repeat_enable;
uniform bool use_albedo_map = false;
uniform float metallic : hint_range(0.0, 1.0) = 0.0;
uniform float roughness : hint_range(0.02, 1.0) = 0.5;
uniform sampler2D orm_map : hint_default_white, filter_linear_mipmap;
uniform bool use_orm_map = false;
uniform sampler2D normal_map : hint_normal, filter_linear_mipmap;
uniform bool use_normal_map = false;
uniform float normal_strength : hint_range(0.0, 2.0) = 1.0;
group_uniforms Detail;
uniform vec3 emission_color : source_color = vec3(0.0);
uniform float emission_energy : hint_range(0.0, 16.0) = 0.0;
uniform float alpha_cutoff : hint_range(0.0, 1.0) = 0.0;
uniform float uv_scale : hint_range(0.01, 16.0) = 1.0;
void fragment() {
    vec2 uv = UV * uv_scale;
    vec4 a = base_color;
    if (use_albedo_map) { a *= texture(albedo_map, uv); }
    ALBEDO = a.rgb;
    ALPHA = a.a;
    METALLIC = metallic;
    ROUGHNESS = roughness;
    AO = 1.0;
    if (use_orm_map) {
        vec3 orm = texture(orm_map, uv).rgb;
        AO = orm.r;
        ROUGHNESS = clamp(roughness * orm.g, 0.02, 1.0);
        METALLIC = clamp(metallic * orm.b, 0.0, 1.0);
    }
    if (use_normal_map) {
        NORMAL_MAP = texture(normal_map, uv).rgb;
        NORMAL_MAP_DEPTH = normal_strength;
    }
    EMISSION = emission_color * emission_energy;
    if (alpha_cutoff > 0.0 && a.a < alpha_cutoff) { discard; }
}
