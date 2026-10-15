shader_type sky;
render_mode use_half_res_pass;
group_uniforms Atmosphere;
uniform vec3 zenith_color : source_color = vec3(0.05, 0.18, 0.46);
uniform vec3 horizon_color : source_color = vec3(0.65, 0.78, 0.94);
uniform vec3 ground_color : source_color = vec3(0.12, 0.13, 0.16);
uniform float atmosphere_power : hint_range(0.2, 8.0) = 1.6;
group_uniforms Sun;
uniform vec3 sun_direction = vec3(0.0, 0.6, 0.8);
uniform vec3 sun_color : source_color = vec3(1.0, 0.87, 0.68);
uniform float sun_size : hint_range(0.001, 0.2) = 0.035;
uniform float sun_intensity : hint_range(0.0, 30.0) = 8.0;
group_uniforms Clouds;
uniform float cloud_amount : hint_range(0.0, 1.0) = 0.3;
uniform float cloud_scale : hint_range(0.1, 20.0) = 3.0;
uniform float cloud_speed : hint_range(0.0, 1.0) = 0.015;
float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }
float noise(vec2 p) {
    vec2 i = floor(p), f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash(i), hash(i + vec2(1.0, 0.0)), f.x),
               mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0)), f.x), f.y);
}
void sky() {
    vec3 dir = normalize(EYEDIR);
    float h = clamp(dir.y, 0.0, 1.0);
    vec3 sky_col = mix(horizon_color, zenith_color, pow(h, atmosphere_power));
    sky_col = mix(ground_color, sky_col, smoothstep(-0.06, 0.04, dir.y));
    vec3 sun_dir = normalize(sun_direction);
    float sun = smoothstep(cos(sun_size * 1.4), cos(sun_size * 0.5), dot(dir, sun_dir));
    float halo = pow(max(dot(dir, sun_dir), 0.0), 64.0);
    sky_col += sun_color * (sun * sun_intensity + halo * sun_intensity * 0.12);
    if (dir.y > 0.01 && cloud_amount > 0.0) {
        vec2 p = dir.xz / max(dir.y + 0.12, 0.15) * cloud_scale + vec2(TIME * cloud_speed);
        float n = 0.55 * noise(p) + 0.3 * noise(p * 2.1) + 0.15 * noise(p * 4.2);
        float cloud = smoothstep(1.0 - cloud_amount, 1.0 - cloud_amount + 0.18, n);
        cloud *= smoothstep(0.0, 0.13, dir.y);
        sky_col = mix(sky_col, vec3(0.92, 0.94, 0.97) + sun_color * halo * 0.1, cloud * 0.85);
    }
    COLOR = sky_col;
}
