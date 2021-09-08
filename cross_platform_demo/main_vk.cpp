#include "vulkanApplication.h"

std::vector<const char *> s_instanceExtensionNames = {
    "VK_EXT_debug_report",
    "VK_EXT_debug_utils",
    "VK_EXT_metal_surface",
    "VK_EXT_swapchain_colorspace",
    "VK_KHR_device_group_creation",
    "VK_KHR_external_fence_capabilities",
    "VK_KHR_external_memory_capabilities",
    "VK_KHR_external_semaphore_capabilities",
    "VK_KHR_get_physical_device_properties2",
    "VK_KHR_get_surface_capabilities2",
    "VK_KHR_surface",
    "VK_MVK_macos_surface"
};

std::vector<const char *> s_layerNames = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_device_simulation",
    "VK_LAYER_KHRONOS_synchronization2"
};


std::vector<const char *> s_deviceExtensionNames = {
    "VK_AMD_gpu_shader_half_float",
    "VK_AMD_negative_viewport_height",
    "VK_AMD_shader_trinary_minmax",
    "VK_EXT_debug_marker",
    "VK_EXT_descriptor_indexing",
    "VK_EXT_fragment_shader_interlock",
    "VK_EXT_hdr_metadata",
    "VK_EXT_host_query_reset",
    "VK_EXT_image_robustness",
    "VK_EXT_inline_uniform_block",
    "VK_EXT_memory_budget",
    "VK_EXT_private_data",
    "VK_EXT_robustness2",
    "VK_EXT_scalar_block_layout",
    "VK_EXT_shader_stencil_export",
    "VK_EXT_shader_viewport_index_layer",
    "VK_EXT_subgroup_size_control",
    "VK_EXT_texel_buffer_alignment",
    "VK_EXT_vertex_attribute_divisor",
    "VK_GOOGLE_display_timing",
    "VK_INTEL_shader_integer_functions2",
    "VK_KHR_16bit_storage",
    "VK_KHR_8bit_storage",
    "VK_KHR_bind_memory2",
    "VK_KHR_create_renderpass2",
    "VK_KHR_dedicated_allocation",
    "VK_KHR_depth_stencil_resolve",
    "VK_KHR_descriptor_update_template",
    "VK_KHR_device_group",
    "VK_KHR_driver_properties",
    "VK_KHR_external_fence",
    "VK_KHR_external_memory",
    "VK_KHR_external_semaphore",
    "VK_KHR_get_memory_requirements2",
    "VK_KHR_image_format_list",
    "VK_KHR_imageless_framebuffer",
    "VK_KHR_maintenance1",
    "VK_KHR_maintenance2",
    "VK_KHR_maintenance3",
    "VK_KHR_multiview",
    "VK_KHR_portability_subset",
    "VK_KHR_push_descriptor",
    "VK_KHR_relaxed_block_layout",
    "VK_KHR_sampler_mirror_clamp_to_edge",
    "VK_KHR_sampler_ycbcr_conversion",
    "VK_KHR_shader_draw_parameters",
    "VK_KHR_shader_float16_int8",
    "VK_KHR_shader_subgroup_extended_types",
    "VK_KHR_storage_buffer_storage_class",
    "VK_KHR_swapchain",
    "VK_KHR_swapchain_mutable_format",
    "VK_KHR_timeline_semaphore",
    "VK_KHR_uniform_buffer_standard_layout",
    "VK_KHR_variable_pointers",
    "VK_NV_glsl_shader"
};

int main()
{
    VulkanApplication* appObj = VulkanApplication::getInstance();
    appObj->initialize();
    appObj->prepare();
    appObj->render();
    appObj->destroy();
    return 0;
}