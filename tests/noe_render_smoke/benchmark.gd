extends Node3D
# Portable smoke scene; reports *main-loop* frame intervals, not GPU timings.
# Run with a real display/GPU before using this as a performance benchmark.

const FRAME_COUNT = 180
var frame_ms = []

func _ready():
    var box = BoxMesh.new()
    for x in range(-4, 5):
        for z in range(-4, 5):
            var mesh = MeshInstance3D.new()
            mesh.mesh = box
            mesh.position = Vector3(x * 1.6, 0, z * 1.6)
            add_child(mesh)
    var camera = Camera3D.new()
    add_child(camera)
    camera.position = Vector3(11, 12, 19)
    camera.look_at(Vector3.ZERO)
    camera.current = true
    var light = DirectionalLight3D.new()
    add_child(light)
    light.rotation_degrees = Vector3(-45, -25, 0)

func _process(delta):
    if delta <= 0.0:
        return
    frame_ms.append(delta * 1000.0)
    if frame_ms.size() < FRAME_COUNT:
        return
    frame_ms.sort()
    var report = {
        "frames": frame_ms.size(),
        "p50_ms": frame_ms[int(frame_ms.size() * 0.50)],
        "p95_ms": frame_ms[int(frame_ms.size() * 0.95)],
        "p99_ms": frame_ms[int(frame_ms.size() * 0.99)],
        "engine": Engine.get_version_info().get("string", "unknown"),
        "warning": "main-loop frame times, not GPU benchmark"
    }
    print("NOERENDER_SMOKE:" + JSON.stringify(report))
    get_tree().quit()
