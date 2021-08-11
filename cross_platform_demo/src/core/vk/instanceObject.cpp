//
// Created by william on 2021/6/19.
//

#include "instanceObject.h"
/**
 * @brief 调试回调
 * @param messageSeverity 警告等级
 * @param messageType 消息类型
 * @param pCallbackData 回调数据
 * @param pUserData 用户定义的指针类型
 * @return
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    (void)messageSeverity;
    (void)messageType;
    (void)pUserData;
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

/**
 * @brief
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pDebugMessenger
 * @return
 */
VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != VK_NULL_HANDLE)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/**
 * @brief
 * @param instance
 * @param debugMessenger
 * @param pAllocator
 */
void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                           "vkDestroyDebugUtilsMessengerEXT");
    if (func != VK_NULL_HANDLE)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

InstanceObject::InstanceObject()
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    if (ENABLE_VALIDATION_LAYERS)
    {
        m_enabledExtensions.emplace_back("VK_LAYER_KHRONOS_validation");
        //        m_enabledExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
        //        m_enabledExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
    // 定义vulkan应用程序的结构体
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    // 定义vulkan实例创建的参数结构体
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = VK_NULL_HANDLE;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_enabledExtensions.size());
        createInfo.ppEnabledLayerNames = reinterpret_cast<const char* const*>(m_enabledExtensions.data());
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = VK_NULL_HANDLE;
    }
    if (vkCreateInstance(&createInfo, m_allocator, &m_handle) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
    setupDebugMessenger();
}

InstanceObject::~InstanceObject()
{
    if (ENABLE_VALIDATION_LAYERS)
    {
        destroyDebugUtilsMessengerEXT(m_handle, m_debugUtilsMessenger, VK_NULL_HANDLE);
    }
    vkDestroyInstance(m_handle, VK_NULL_HANDLE);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

std::vector<const char*> InstanceObject::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

bool InstanceObject::checkValidationLayerSupport()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceLayerProperties(&extensionCount, VK_NULL_HANDLE);
    RAS_INFO("extensionCount:%d\n", extensionCount);
    std::vector<VkLayerProperties> availableLayers(extensionCount);
    vkEnumerateInstanceLayerProperties(&extensionCount, availableLayers.data());
    for (auto& layer : availableLayers)
    {
        RAS_INFO(layer.layerName);
    }
    for (const auto& layerName : m_enabledExtensions)
    {
        bool isFound = std::any_of(availableLayers.begin(), availableLayers.end(),
                                   [&](VkLayerProperties layerProperties) {
                                       return std::string_view(layerName) == layerProperties.layerName;
                                   });
        if (!isFound)
        {
            return false;
        }
    }
    return true;
}

void InstanceObject::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    /// messageSeverity字段允许你指定你的回调函数在何种严重等级下被触发
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pNext = VK_NULL_HANDLE;
    createInfo.pUserData = VK_NULL_HANDLE;
}

void InstanceObject::setupDebugMessenger()
{
    if (!ENABLE_VALIDATION_LAYERS)
    {
        return;
    }
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (createDebugUtilsMessengerEXT(m_handle, &createInfo, VK_NULL_HANDLE, &m_debugUtilsMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
