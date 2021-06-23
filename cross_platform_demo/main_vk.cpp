#include "base.h"
#include "logger.h"
#include "triangleVk.h"
#include "vulkanApplications.h"

std::vector<const char*> instanceExtensionNames = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};

std::vector<const char*> layerNames = {
    "VK_LAYER_LUNARG_api_dump"
};

int main()
{
    //    TriangleVk triangle;
    //    triangle.run();
    VulkanApplications applications;
    applications.initialize();
    return 0;
}