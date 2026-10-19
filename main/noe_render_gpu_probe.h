// NoeRender Vulkan capability snapshot: no display, global state or telemetry.
#ifndef NOE_RENDER_GPU_PROBE_H
#define NOE_RENDER_GPU_PROBE_H
#include "core/string/ustring.h"
#include <cstdint>
namespace NoeRender {
struct GpuCapabilities {
    bool probed = false;
    bool loader_available = false;
    bool instance_available = false;
    bool graphics_compute_queue = false;
    bool render_device_candidate = false;
    bool swapchain_extension = false;
    bool software_device = false;
    uint32_t vendor_id = 0;
    uint32_t device_id = 0;
    uint32_t api_version = 0;
    uint64_t device_local_bytes = 0;
    String device_name;
    String reason;
};
GpuCapabilities probe_vulkan();
} // namespace NoeRender
#endif
