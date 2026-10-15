shader_type particles;
render_mode keep_data;
uniform float radial_speed : hint_range(0.0, 30.0) = 8.0;
uniform float upward_bias : hint_range(-1.0, 2.0) = 0.25;
uniform float gravity : hint_range(0.0, 30.0) = 9.8;
uniform vec4 impact_color : source_color = vec4(1.0, 0.65, 0.2, 1.0);
void start() {
    vec3 direction = vec3(rand_from_seed(RANDOM_SEED) * 2.0 - 1.0, rand_from_seed(RANDOM_SEED) * 2.0 - 1.0 + upward_bias, rand_from_seed(RANDOM_SEED) * 2.0 - 1.0);
    VELOCITY = normalize(direction + vec3(0.001)) * radial_speed;
    COLOR = impact_color;
}
void process() {
    VELOCITY.y -= gravity * DELTA;
    float life = clamp(CUSTOM.y / max(LIFETIME, 0.001), 0.0, 1.0);
    COLOR = vec4(impact_color.rgb, impact_color.a * (1.0 - life));
    TRANSFORM[3].xyz += VELOCITY * DELTA;
}
