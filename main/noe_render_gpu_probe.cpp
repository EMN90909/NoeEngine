// Early, best-effort Vulkan physical-device probe. Real renderer still validates
// present/swapchain/extensions with the selected DisplayServer. This probe must
// never override a user's explicit CLI driver or edit project.godot.
#include "noe_render_gpu_probe.h"
#ifdef VULKAN_ENABLED
#include "drivers/vulkan/godot_vulkan.h"
#include <vector>
#endif

namespace NoeRender {
GpuCapabilities probe_vulkan() {
    GpuCapabilities result;
#ifdef VULKAN_ENABLED
#if defined(MACOS_ENABLED) || defined(IOS_ENABLED)
    // MoltenVK needs portability enumeration. Defer validation to the real
    // rendering backend on Apple platforms instead of reporting a false negative.
    result.reason = "MoltenVK device enumeration deferred to native backend";
    return result;
#else
    result.probed = true;
#ifdef USE_VOLK
    if (volkInitialize() != VK_SUCCESS) {
        result.reason = "Vulkan loader unavailable";
        return result;
    }
#endif
    result.loader_available = true;
    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "NoeRender capability probe";
    app.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo ci = {};
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &app;
    VkInstance instance = VK_NULL_HANDLE;
    const VkResult created = vkCreateInstance(&ci, nullptr, &instance);
    if (created != VK_SUCCESS || instance == VK_NULL_HANDLE) {
        result.reason = "Unable to create minimum Vulkan instance (driver/ICD)";
        return result;
    }
    result.instance_available = true;
#ifdef USE_VOLK
    volkLoadInstance(instance);
#endif
    uint32_t count = 0;
    VkResult error = vkEnumeratePhysicalDevices(instance, &count, nullptr);
    if (error != VK_SUCCESS || !count || count > 256) {
        result.reason = "No Vulkan physical devices enumerated";
        vkDestroyInstance(instance, nullptr);
        return result;
    }
    std::vector<VkPhysicalDevice> devices(count);
    error = vkEnumeratePhysicalDevices(instance, &count, devices.data());
    if (error != VK_SUCCESS) {
        result.reason = "Vulkan physical-device enumeration failed";
        vkDestroyInstance(instance, nullptr);
        return result;
    }
    for (uint32_t i = 0; i < count; ++i) {
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(devices[i], &props);
        uint32_t queue_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_count, nullptr);
        if (!queue_count || queue_count > 256) continue;
        std::vector<VkQueueFamilyProperties> families(queue_count);
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_count, families.data());
        bool capable = false;
        for (const auto &family : families) {
            if (family.queueCount && (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                    (family.queueFlags & VK_QUEUE_COMPUTE_BIT)) capable = true;
        }
        if (!capable) continue;
        VkPhysicalDeviceMemoryProperties mem = {};
        vkGetPhysicalDeviceMemoryProperties(devices[i], &mem);
        result.graphics_compute_queue = true;
        result.render_device_candidate = true;
        result.vendor_id = props.vendorID;
        result.device_id = props.deviceID;
        result.api_version = props.apiVersion;
        result.device_name = String::utf8(props.deviceName);
        for (uint32_t h = 0; h < mem.memoryHeapCount; ++h) {
            if (mem.memoryHeaps[h].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                result.device_local_bytes += mem.memoryHeaps[h].size;
            }
        }
        result.reason = "Vulkan graphics/compute device found; presentation not yet validated";
        break;
    }
    if (!result.render_device_candidate) result.reason = "No Vulkan device with graphics and compute queues";
    vkDestroyInstance(instance, nullptr);
    return result;
#endif
#else
    result.reason = "Vulkan renderer not compiled";
    return result;
#endif
}
} // namespace NoeRender
