#include "vulkanApplication.h"

std::vector<const char *>  instanceExtensionNames = {
    VK_KHR_SURFACE_EXTENSION_NAME,
};

std::vector<const char *> layerNames = {
    "VK_LAYER_LUNARG_api_dump"
};

std::vector<const char *> deviceExtensionNames = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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