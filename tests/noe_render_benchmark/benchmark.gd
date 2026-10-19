extends Node3D
# Controlled rendered 3D benchmark scene, not a GPU timestamp profiler.
const WARMUP_FRAMES = 100
const SAMPLE_FRAMES = 240
var frame_number = 0
var samples = []
var material = StandardMaterial3D.new()

func _ready():
    material.albedo_color = Color(0.45, 0.37, 0.55, 1)
    material.roughness = 0.35
    material.metallic = 0.15
    var mesh = SphereMesh.new()
    mesh.radial_segments = 16
    mesh.rings = 8
    for x in range(-9, 10):
        for z in range(-9, 10):
            var item = MeshInstance3D.new()
            item.mesh = mesh
            item.material_override = material
            item.position = Vector3(x * 1.5, sin(float(x + z) * 0.2), z * 1.5)
            add_child(item)
    for index in range(9):
        var light = OmniLight3D.new()
        light.position = Vector3((index % 3 - 1) * 8.0, 5.0, (int(index / 3) - 1) * 8.0)
        light.light_energy = 0.65
        light.omni_range = 18.0
        add_child(light)
    var camera = Camera3D.new()
    camera.position = Vector3(19, 20, 30)
    add_child(camera)
    camera.look_at(Vector3(0, 0, 0))
    camera.current = true
    var ambient = WorldEnvironment.new()
    var environment = Environment.new()
    environment.background_mode = Environment.BG_COLOR
    environment.background_color = Color(0.06, 0.08, 0.12)
    ambient.environment = environment
    add_child(ambient)

func _process(delta):
    frame_number += 1
    if frame_number <= WARMUP_FRAMES:
        return
    if delta > 0.0 and delta < 1.0:
        samples.append(delta * 1000.0)
    if samples.size() < SAMPLE_FRAMES:
        return
    samples.sort()
    var count = samples.size()
    var results = {
        "frames": count,
        "scene": "361 spheres + 9 omni lights",
        "frame_ms_p50": samples[int((count - 1) * 0.50)],
        "frame_ms_p95": samples[int((count - 1) * 0.95)],
        "frame_ms_p99": samples[int((count - 1) * 0.99)],
        "draw_calls_last_frame": RenderingServer.get_rendering_info(RenderingServer.RENDERING_INFO_TOTAL_DRAW_CALLS_IN_FRAME),
        "primitives_last_frame": RenderingServer.get_rendering_info(RenderingServer.RENDERING_INFO_TOTAL_PRIMITIVES_IN_FRAME),
        "video_memory_estimate_bytes": RenderingServer.get_rendering_info(RenderingServer.RENDERING_INFO_VIDEO_MEM_USED),
        "note": "application frame times, GPU timestamps not recorded"
    }
    var screenshot_path = OS.get_environment("NOERENDER_SCREENSHOT_PATH")
    if screenshot_path != "":
        var image = get_viewport().get_texture().get_image()
        results["screenshot_error"] = image.save_png(screenshot_path)
    print("NOERENDER_BENCHMARK:" + JSON.stringify(results))
    get_tree().quit()
