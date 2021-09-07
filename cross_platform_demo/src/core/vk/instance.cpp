//
// Created by william on 2021/6/19.
//

#include "instance.h"

#include "physicalDevice.h"
/**
 * @brief 调试回调
 * @param messageSeverity 警告等级
 * @param messageType 消息类型
 * @param pCallbackData 回调数据
 * @param pUserData 用户定义的指针类型
 * @return
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugUtilMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                             const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* pUserData)
{
    (void)pUserData;
    // Log debug messge
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LOG_WARN("{} - {}: {}", callbackData->messageIdNumber, callbackData->pMessageIdName, callbackData->pMessage);
    }
    else if (messageType & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        LOG_ERROR("{} - {}: {}", callbackData->messageIdNumber, callbackData->pMessageIdName, callbackData->pMessage);
    }
    return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*type*/,
                                                    uint64_t /*object*/, size_t /*location*/, int32_t /*message_code*/,
                                                    const char* layerPrefix, const char* message, void* /*user_data*/)
{
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("{}: {}", layerPrefix, message);
    }
    else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARN("{}: {}", layerPrefix, message);
    }
    else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_WARN("{}: {}", layerPrefix, message);
    }
    else
    {
        LOG_INFO("{}: {}", layerPrefix, message);
    }
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
            LOG_ERROR("Validation Layer %s not found", layer);
            return false;
        }
    }
}

std::vector<const char*> getOptimalValidationLayers(const std::vector<VkLayerProperties>& supportedInstanceLayers)
{
    std::vector<std::vector<const char*>> validationLayerPriorityList = {
        /// 首选的验证层是"VK_LAYER_KHRONOS_validation"
        { "VK_LAYER_KHRONOS_validation" },
        /// 否则我们会退回到使用LunarG基础层
        { "VK_LAYER_LUNARG_standard_validation" },
        /// 否则我们将尝试启用组成LunarG元层的各个层，一旦它不存在。
        {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_GOOGLE_unique_objects",
        },
        /// 否则，作为最后的手段，我们将退回到尝试启用LunarG核心层
        { "VK_LAYER_LUNARG_core_validation" }
    };

    for (auto& validationLayers : validationLayerPriorityList)
    {
        if (validateLayers(validationLayers, supportedInstanceLayers))
        {
            return validationLayers;
        }
        LOG_WARN("Couldn't enable validation layers (see log for error) - falling back");
    }
    return {};
}

Instance::Instance()
{
    /// 获取支持的扩展
    uint32_t instanceExtensionCount;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));
    std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data()));
//    if (ENABLE_VALIDATION_LAYERS)
//    {
//        bool debugUtils = false;
//        bool headlessExtension = false;
//        for (auto& availableExtension : availableInstanceExtensions)
//        {
//            if (strcmp(availableExtension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
//            {
//                debugUtils = true;
//                LOG_INFO("%s is available, enabling it", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//                m_enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//            }
//            if (strcmp(availableExtension.extensionName, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME) == 0)
//            {
//                headlessExtension = true;
//                LOG_INFO("%s is available, enabling it", VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
//                m_enabledExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
//            }
//            if (strcmp(availableExtension.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0)
//            {
//                LOG_INFO("%s is available, enabling it", VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
//                m_enabledExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
//            }
//        }
//        if (!debugUtils)
//        {
//            m_enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
//        }
//        if (!headlessExtension)
//        {
//            LOG_WARN("%s is not available, disabling swapchain creation", VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
//        }
//    }

//    auto extensionError = VK_FALSE;
//    for (auto extension : m_enabledExtensions)
//    {
//        if (std::find_if(availableInstanceExtensions.begin(), availableInstanceExtensions.end(),
//                         [&extension](VkExtensionProperties availableExtension) { return strcmp(availableExtension.extensionName, extension) == 0; }) == availableInstanceExtensions.end())
//        {
//            LOG_ERROR("Required instance extension {} not available, cannot run", extension);
//        }
//    }
//    if (extensionError)
//    {
//        throw std::runtime_error("Required instance extensions are missing.");
//    }

    /// 获取支持的层
//    uint32_t instanceLayerCount;
//    VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
//    std::vector<VkLayerProperties> supportedValidationLayers(instanceLayerCount);
//    VK_CHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, supportedValidationLayers.data()));
//    if (ENABLE_VALIDATION_LAYERS)
//    {
//        // Determine the optimal validation layers to enable that are necessary for useful debugging
//        std::vector<const char*> optimalValidationLayers = getOptimalValidationLayers(supportedValidationLayers);
//        m_validationLayers.insert(m_validationLayers.end(), optimalValidationLayers.begin(), optimalValidationLayers.end());
//    }
//
//    if (validateLayers(m_validationLayers, supportedValidationLayers))
//    {
//        LOG_INFO("Enabled Validation Layers:");
//        std::for_each(m_validationLayers.begin(), m_validationLayers.end(), [](const char* validationLayer) {
//            LOG_INFO("%s", validationLayer);
//        });
//    }
//    else
//    {
//        throw std::runtime_error("Required validation layers are missing.");
//    }
    // 定义vulkan应用程序的结构体
//    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
//    appInfo.pApplicationName = "triangle";
//    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.pEngineName = "Vulkan Demo";
//    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//    appInfo.apiVersion = VK_API_VERSION_1_0;
//    // 定义vulkan实例创建的参数结构体
//    VkInstanceCreateInfo instanceInfo{};
//    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//    instanceInfo.pNext = VK_NULL_HANDLE;
//    instanceInfo.pApplicationInfo = &appInfo;
//    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(m_enabledExtensions.size());
//    instanceInfo.ppEnabledExtensionNames = reinterpret_cast<const char* const*>(m_enabledExtensions.data());
//    if (ENABLE_VALIDATION_LAYERS)
//    {
//        instanceInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
//        instanceInfo.ppEnabledLayerNames = reinterpret_cast<const char* const*>(m_validationLayers.data());
//        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
//        populateDebugMessengerCreateInfo(debugCreateInfo);
//        instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
//    }
//    else
//    {
//        instanceInfo.enabledLayerCount = 0;
//        instanceInfo.pNext = VK_NULL_HANDLE;
//    }
//    if (vkCreateInstance(&instanceInfo, m_allocator, &m_handle) != VK_SUCCESS)
//    {
//        throw std::runtime_error("failed to create instance!");
//    }
//    setupDebugMessenger();
}

Instance::~Instance()
{
    if (ENABLE_VALIDATION_LAYERS)
    {
        destroyDebugUtilsMessengerEXT(m_handle, m_debugUtilsMessenger, VK_NULL_HANDLE);
    }
    vkDestroyInstance(m_handle, VK_NULL_HANDLE);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Instance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsCreateInfo)
{
    debugUtilsCreateInfo = {};
    debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    /// messageSeverity字段允许你指定你的回调函数在何种严重等级下被触发
    debugUtilsCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    debugUtilsCreateInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsCreateInfo.pfnUserCallback = debugUtilMessageCallback;
    debugUtilsCreateInfo.pNext = VK_NULL_HANDLE;
    debugUtilsCreateInfo.pUserData = VK_NULL_HANDLE;
}

void Instance::setupDebugMessenger()
{
    if (ENABLE_VALIDATION_LAYERS)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (createDebugUtilsMessengerEXT(m_handle, &createInfo, VK_NULL_HANDLE, &m_debugUtilsMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }
}

PhysicalDevice& Instance::getSuitableGpu(VkSurfaceKHR surface)
{
    assert(!m_gpus.empty() && "No physical devices were found on the system.");
    for (auto& gpu : m_gpus)
    {
        if (gpu->properties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            // See if it work with the surface
            size_t queueCount = gpu->queueFamilyProperties().size();
            for (uint32_t queueIdx = 0; static_cast<size_t>(queueIdx) < queueCount; queueIdx++)
            {
                if (gpu->isPresentSupported(surface, queueIdx))
                {
                    return *gpu;
                }
            }
        }
    }

    LOG_WARN("Couldn't find a discrete physical device, picking default GPU");
    return *m_gpus.at(0);
}

PhysicalDevice& Instance::getFirstGpu()
{
    assert(!m_gpus.empty() && "No physical devices were found on the system.");

    // 找到一个DISCRETE的GPU
    for (auto& gpu : m_gpus)
    {
        if (gpu->properties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return *gpu;
        }
    }

    LOG_WARN("Couldn't find a discrete physical device, picking default GPU");
    return *m_gpus.at(0);
}

bool Instance::isEnabled(const char* extension) const
{
    return std::find_if(m_layerExtension.m_requestedExtensionNames.begin(), m_layerExtension.m_requestedExtensionNames.end(), [extension](const char* enabledExtension) { return strcmp(extension, enabledExtension) == 0; }) != m_layerExtension.m_requestedExtensionNames.end();
}

VkResult Instance::createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName)
{
    m_layerExtension.m_requestedLayerNames = layers;
    m_layerExtension.m_requestedExtensionNames = extensions;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = applicationName;
    appInfo.engineVersion = 1;
    // VK_API_VERSION is now deprecated, use VK_MAKE_VERSION instead.
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    // 定义instance CI
    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;

    instanceInfo.enabledLayerCount = (uint32_t)layers.size();
    instanceInfo.ppEnabledLayerNames = !layers.empty() ? extensions.data() : nullptr;

    instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
    instanceInfo.ppEnabledExtensionNames = !extensions.empty() ? extensions.data() : nullptr;
    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_handle);
    ASSERT(result == VK_SUCCESS);
    return result;
}