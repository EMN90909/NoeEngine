// NoeCore runtime hooks are advisory: never rewrite saved graphics preferences.
#ifndef NOE_CORE_INTEGRATION_H
#define NOE_CORE_INTEGRATION_H

#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "core/string/print_string.h"
#include "main/noe_core_policy.h"
#include "main/noe_render_gpu_probe.h"
#include "main/noe_render_quality.h"

namespace NoeCore {
inline void initialize(const NoeRender::GpuCapabilities &gpu) {
    GLOBAL_DEF_BASIC("performance/noecore/diagnostics/startup_benchmark", false);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "performance/noecore/memory/target_cache_budget_mb",
            PROPERTY_HINT_RANGE, "0,65536,64"), 0);
    GLOBAL_DEF_BASIC("performance/noecore/memory/safe_eviction", true);

    Hardware h;
    h.cpu_threads = static_cast<uint32_t>(OS::get_singleton()->get_processor_count());
    h.ram_bytes = NoeRender::physical_ram_bytes();
    h.vram_bytes = gpu.device_local_bytes; // Vulkan heap is only an estimate, not free VRAM.
    h.hardware_gpu = gpu.render_device_candidate && !gpu.software_device;
    h.graphics_api_supported = gpu.render_device_candidate;
    if (bool(GLOBAL_GET("performance/noecore/diagnostics/startup_benchmark"))) {
        // Diagnostics omit paths, project data, tokens, model and device identifiers.
        print_line(vformat("NoeCore hardware: threads=%d RAM_MiB=%d GPU_heap_MiB=%d verified_vulkan=%s recommendation=%s (advisory only)",
                static_cast<int>(h.cpu_threads), static_cast<int64_t>(h.ram_bytes / (1024ULL * 1024)), static_cast<int64_t>(h.vram_bytes / (1024ULL * 1024)),
                h.graphics_api_supported ? "yes" : "no", String(profile_name(recommend(h)))));
    }
}
} // namespace NoeCore
#endif // NOE_CORE_INTEGRATION_H
