shader_type particles;
render_mode keep_data;
uniform float radial_speed : hint_range(0.0, 30.0) = 8.0;
uniform float upward_bias : hint_range(-1.0, 2.0) = 0.25;
uniform float gravity : hint_range(0.0, 30.0) = 9.8;
uniform vec4 impact_color : source_color = vec4(1.0, 0.65, 0.2, 1.0);
float noe_rand(inout uint seed) {
    seed = seed * 1664525u + 1013904223u;
    return float(seed & 0x00ffffffu) / 16777216.0;
}
void start() {
    uint seed = RANDOM_SEED;
    vec3 direction = vec3(noe_rand(seed) * 2.0 - 1.0, noe_rand(seed) * 2.0 - 1.0 + upward_bias, noe_rand(seed) * 2.0 - 1.0);
    VELOCITY = normalize(direction + vec3(0.001)) * radial_speed;
    CUSTOM.y = 0.0;
    COLOR = impact_color;
}
void process() {
    VELOCITY.y -= gravity * DELTA;
    CUSTOM.y += DELTA;
    float life = clamp(CUSTOM.y / max(LIFETIME, 0.001), 0.0, 1.0);
    COLOR = vec4(impact_color.rgb, impact_color.a * (1.0 - life));
    TRANSFORM[3].xyz += VELOCITY * DELTA;
}
