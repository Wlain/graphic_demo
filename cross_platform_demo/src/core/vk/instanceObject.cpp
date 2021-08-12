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

bool validateLayers(const std::vector<const char*>& required, const std::vector<VkLayerProperties>& available)
{
    for (auto layer : required)
    {
        bool found = std::any_of(available.begin(), available.end(), [&](VkLayerProperties properties) {
            return (strcmp(properties.layerName, layer) == 0);
        });
        if (found)
        {
            return true;
        }
        else
        {
            RAS_ERROR("Validation Layer {} not found", layer);
            return false;
        }
    }
}

std::vector<const char*> getOptimalValidationLayers(const std::vector<VkLayerProperties>& supportedInstanceLayers)
{
    std::vector<std::vector<const char*>> validationLayerPriorityList = {
        /// The preferred validation layer is "VK_LAYER_KHRONOS_validation"
        { "VK_LAYER_KHRONOS_validation" },
        /// Otherwise we fallback to using the LunarG meta layer
        { "VK_LAYER_LUNARG_standard_validation" },
        /// Otherwise we attempt to enable the individual layers that compose the LunarG meta layer since it doesn't exist
        {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_GOOGLE_unique_objects",
        },
        /// Otherwise as a last resort we fallback to attempting to enable the LunarG core layer
        { "VK_LAYER_LUNARG_core_validation" }
    };

    for (auto& validationLayers : validationLayerPriorityList)
    {
        if (validateLayers(validationLayers, supportedInstanceLayers))
        {
            return validationLayers;
        }
        RAS_WARN("Couldn't enable validation layers (see log for error) - falling back");
    }
    return {};
}

InstanceObject::InstanceObject()
{
    /// 获取支持的扩展
    uint32_t instanceExtensionCount;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));
    std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data()));
    if (ENABLE_VALIDATION_LAYERS)
    {
        bool debugUtils = false;
        bool headlessExtension = false;
        for (auto& availableExtension : availableInstanceExtensions)
        {
            if (strcmp(availableExtension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
            {
                debugUtils = true;
                RAS_INFO("%s is available, enabling it", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                m_enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
            if (strcmp(availableExtension.extensionName, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME) == 0)
            {
                headlessExtension = true;
                RAS_INFO("%s is available, enabling it", VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
                m_enabledExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
            }
            if (strcmp(availableExtension.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0)
            {
                RAS_INFO("%s is available, enabling it", VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
                m_enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            }
        }
        if (!debugUtils)
        {
            m_enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        if (!headlessExtension)
        {
            RAS_WARN("{} is not available, disabling swapchain creation", VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
        }
    }

    /// 获取支持的层
    uint32_t instanceLayerCount;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
    std::vector<VkLayerProperties> supportedValidationLayers(instanceLayerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, supportedValidationLayers.data()));
    if (ENABLE_VALIDATION_LAYERS)
    {
        // Determine the optimal validation layers to enable that are necessary for useful debugging
        std::vector<const char*> optimalValidationLayers = getOptimalValidationLayers(supportedValidationLayers);
        m_validationLayers.insert(m_validationLayers.end(), optimalValidationLayers.begin(), optimalValidationLayers.end());
    }

    if (validateLayers(m_validationLayers, supportedValidationLayers))
    {
        RAS_INFO("Enabled Validation Layers:");
        for (const auto& layer : m_validationLayers)
        {
            RAS_INFO("%s", layer);
        }
    }
    else
    {
        throw std::runtime_error("Required validation layers are missing.");
    }

    // 定义vulkan应用程序的结构体
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.pApplicationName = "triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    // 定义vulkan实例创建的参数结构体
    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = VK_NULL_HANDLE;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(m_enabledExtensions.size());
    instanceInfo.ppEnabledExtensionNames = reinterpret_cast<const char* const*>(m_enabledExtensions.data());
    if (ENABLE_VALIDATION_LAYERS)
    {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        instanceInfo.ppEnabledLayerNames = reinterpret_cast<const char* const*>(m_validationLayers.data());
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);
        instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        instanceInfo.enabledLayerCount = 0;
        instanceInfo.pNext = VK_NULL_HANDLE;
    }
    if (vkCreateInstance(&instanceInfo, m_allocator, &m_handler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
    setupDebugMessenger();
}

InstanceObject::~InstanceObject()
{
    if (ENABLE_VALIDATION_LAYERS)
    {
        destroyDebugUtilsMessengerEXT(m_handler, m_debugUtilsMessenger, VK_NULL_HANDLE);
    }
    vkDestroyInstance(m_handler, VK_NULL_HANDLE);
    glfwDestroyWindow(m_window);
    glfwTerminate();
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

    if (createDebugUtilsMessengerEXT(m_handler, &createInfo, VK_NULL_HANDLE, &m_debugUtilsMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
