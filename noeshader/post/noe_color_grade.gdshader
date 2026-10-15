shader_type canvas_item;
// Apply to a full-screen ColorRect. Supports live inspector edits.
uniform sampler2D screen_texture : hint_screen_texture, filter_linear;
group_uniforms Grade;
uniform float exposure : hint_range(-5.0, 5.0) = 0.0;
uniform float contrast : hint_range(0.0, 3.0) = 1.0;
uniform float saturation : hint_range(0.0, 3.0) = 1.0;
uniform vec3 lift : source_color = vec3(0.0);
uniform vec3 gamma : source_color = vec3(1.0);
uniform vec3 gain : source_color = vec3(1.0);
group_uniforms Lens;
uniform float vignette : hint_range(0.0, 2.0) = 0.2;
uniform float chromatic_aberration : hint_range(0.0, 0.02) = 0.001;
uniform float bloom_blend : hint_range(0.0, 1.0) = 0.15;
uniform float bloom_threshold : hint_range(0.0, 2.0) = 0.7;
uniform float bloom_radius : hint_range(0.0, 0.02) = 0.003;
vec3 sample_color(vec2 uv) { return texture(screen_texture, clamp(uv, vec2(0.001), vec2(0.999))).rgb; }
void fragment() {
    vec2 uv = SCREEN_UV;
    vec2 shift = (uv - 0.5) * chromatic_aberration;
    vec3 color = vec3(sample_color(uv + shift).r, sample_color(uv).g, sample_color(uv - shift).b);
    vec3 blur = vec3(0.0);
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            vec3 s = sample_color(uv + vec2(float(x), float(y)) * bloom_radius);
            blur += max(s - vec3(bloom_threshold), vec3(0.0));
        }
    }
    blur /= 25.0;
    color += blur * bloom_blend;
    color *= exp2(exposure);
    color = max((color + lift) * gain, vec3(0.0));
    color = pow(color, vec3(1.0) / max(gamma, vec3(0.01)));
    color = (color - 0.5) * contrast + 0.5;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    color = mix(vec3(luma), color, saturation);
    vec2 center = uv - vec2(0.5);
    float edge = smoothstep(0.15, 0.75, dot(center, center) * 2.0);
    color *= 1.0 - edge * vignette;
    COLOR = vec4(max(color, vec3(0.0)), 1.0);
}
