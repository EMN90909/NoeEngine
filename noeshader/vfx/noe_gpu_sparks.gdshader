shader_type particles;
// Assign as a GPUParticles3D process material; use an emitting mesh material for visuals.
render_mode keep_data;
uniform float launch_speed : hint_range(0.0, 40.0) = 9.0;
uniform float spread : hint_range(0.0, 1.0) = 0.65;
uniform float gravity_strength : hint_range(0.0, 40.0) = 14.0;
uniform float drag : hint_range(0.0, 10.0) = 1.2;
uniform vec4 spark_color : source_color = vec4(1.0, 0.45, 0.08, 1.0);
void start() {
    vec3 jitter = vec3(rand_from_seed(RANDOM_SEED), rand_from_seed(RANDOM_SEED), rand_from_seed(RANDOM_SEED)) * 2.0 - 1.0;
    vec3 heading = normalize(mix(vec3(0.0, 1.0, 0.0), normalize(jitter + vec3(0.001)), spread));
    VELOCITY = heading * launch_speed * mix(0.6, 1.2, rand_from_seed(RANDOM_SEED));
    COLOR = spark_color;
}
void process() {
    VELOCITY += vec3(0.0, -gravity_strength, 0.0) * DELTA;
    VELOCITY *= exp(-drag * DELTA);
    float life = clamp(CUSTOM.y / max(LIFETIME, 0.001), 0.0, 1.0);
    COLOR = vec4(spark_color.rgb, spark_color.a * (1.0 - life));
}
