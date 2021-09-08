//
// Created by william on 2021/9/6.
//

#include "layerAndExtension.h"

#include "vulkanApplication.h"
LayerAndExtension::LayerAndExtension() = default;

LayerAndExtension::~LayerAndExtension()
{
    m_debugReportCallback = VK_NULL_HANDLE;
    m_dbgCreateDebugReportCallback = VK_NULL_HANDLE;
    m_dbgDestroyDebugReportCallback = VK_NULL_HANDLE;
}

VkResult LayerAndExtension::getInstanceLayerProperties()
{
    uint32_t instanceLayerCount{};
    std::vector<VkLayerProperties> layerProperties;
    VkResult result;
    // 查询所有的层
    do
    {
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        if (result)
        {
            return result;
        }
        if (instanceLayerCount == 0)
            return VK_INCOMPLETE; // 获取失败
        layerProperties.resize(instanceLayerCount);
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
    } while (result == VK_INCOMPLETE);
    // 查询每个层的所有扩展信息，并进行保存
    std::cout << "\nInstanced Layers\n"
              << std::endl;
    std::cout << "==================" << std::endl;
    for (auto globalLayerProp : layerProperties)
    {
        // 显示层的名字和描述信息
        std::cout << "\n"
                  << globalLayerProp.description << "\n\t|\n\t|---[Layer Name]-->" << globalLayerProp.layerName << "\n";
        LayerProperties layerProps;
        layerProps.properties = globalLayerProp;
        // 根据属性信息，获取就要实例的扩展
        result = getExtensionProperties(layerProps);
        if (result)
        {
            continue;
        }
        m_layerPropertyList.push_back(layerProps);
        // 显示每个实例层的扩展名称
        for (auto j : layerProps.extensions)
        {
            std::cout << "\t\t|\n\t\t---[Layer Extension]---> " << j.extensionName << "\n";
        }
    }
    return result;
}

/// 此函数在实例中检索扩展及其属性和设备级别。传递有效的物理设备
/// 指向检索设备级扩展的指针，否则使用 NULL 来检索特定于实例级别的扩展。
VkResult LayerAndExtension::getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu)
{
    uint32_t extensionCount{};
    VkResult result;
    char* layerName = layerProps.properties.layerName;
    do
    {
        if (gpu)
        {
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, nullptr);
        }
        else
        {
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);
        }
        if (result || extensionCount == 0)
        {
            continue;
        }
        layerProps.extensions.resize(extensionCount);
        if (gpu)
        {
            result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
        }
        else
        {
            result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
        }
    } while (result == VK_INCOMPLETE);
    return result;
}

/// 获取设备的扩展
VkResult LayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
    VkResult result = VK_ERROR_INITIALIZATION_FAILED;
    // 对每一个层查询所有扩展信息，并进行保存
    std::cout << "\nDevice extensions\n"
              << std::endl;
    std::cout << "==================" << std::endl;
    VulkanApplication* appObj = VulkanApplication::getInstance();
    // 取地址，避免拷贝
    std::vector<LayerProperties>* instanceLayerProp = &appObj->getInstance()->m_instanceObj.m_layerExtension.m_layerPropertyList;
    for (auto globalLayerProp : *instanceLayerProp)
    {
        LayerProperties layerProps;
        layerProps.properties = globalLayerProp.properties;
        if (result == getExtensionProperties(layerProps, gpu))
        {
            continue;
        }
        std::cout << "\n"
                  << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
        m_layerPropertyList.push_back(layerProps);
        if (!layerProps.extensions.empty())
        {
            for (auto e : layerProps.extensions)
                std::cout << "\t\t|\n\t\t|---[Device Extesion]--> " << e.extensionName << "\n";
        }
        else
        {
            std::cout << "\t\t|\n\t\t|---[Device Extesion]--> No extension found \n";
        }
    }
    return result;
}

bool LayerAndExtension::isLayersSupported(std::vector<const char*>& layerNames)
{
    auto checkCount = (uint32_t)layerNames.size();
    auto layerCount = (uint32_t)m_layerPropertyList.size();
    std::vector<const char*> unSupportLayerNames;
    for (uint32_t i = 0; i < checkCount; i++) {
		VkBool32 isSupported = 0;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (!strcmp(layerNames[i], m_layerPropertyList[j].properties.layerName)) {
				isSupported = 1;
			}
		}

		if (!isSupported) {
			std::cout << "No Layer support found, removed from layer: " << layerNames[i] << std::endl;
			unSupportLayerNames.push_back(layerNames[i]);
		}
		else {
			std::cout << "Layer supported: " << layerNames[i] << std::endl;
		}
	}

	for (auto i : unSupportLayerNames) {
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end()) layerNames.erase(it);
	}

	return true;
}


VKAPI_ATTR VkBool32 VKAPI_CALL LayerAndExtension::debugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
                                                                uint64_t srcObject, size_t location, int32_t msgCode,
                                                                const char* layerPrefix, const char* msg, void* userData)
{
    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        std::cout << "[VK_DEBUG_REPORT] ERROR: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
    }
    else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        std::cout << "[VK_DEBUG_REPORT] WARNING: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
    }
    else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
        std::cout << "[VK_DEBUG_REPORT] INFORMATION: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
    }
    else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        std::cout << "[VK_DEBUG_REPORT] PERFORMANCE: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
    }
    else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
        std::cout << "[VK_DEBUG_REPORT] DEBUG: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
    }
    else
    {
        return VK_FALSE;
    }

    fflush(stdout);
    return VK_TRUE;
}

VkResult LayerAndExtension::createDebugReportCallback()
{
    VkResult result;
    VulkanApplication* appObj = VulkanApplication::getInstance();
    auto& instance = appObj->m_instanceObj;
    // Get vkCreateDebugReportCallbackEXT API
    m_dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (! m_dbgCreateDebugReportCallback) {
        std::cout << "Error: GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function." << std::endl;
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    std::cout << "GetInstanceProcAddr loaded dbgCreateDebugReportCallback function\n";

    // Get vkDestroyDebugReportCallbackEXT API
    m_dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (! m_dbgDestroyDebugReportCallback) {
        std::cout << "Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function." << std::endl;
        return VK_ERROR_INITIALIZATION_FAILED;
    }
    std::cout << "GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function\n";

    // Define the debug report control structure, provide the reference of 'debugFunction'
    // , this function prints the debug information on the console.
    m_dbgReportCreateInfo.sType		= VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    m_dbgReportCreateInfo.pfnCallback = debugFunction;
    m_dbgReportCreateInfo.pUserData	= nullptr;
    m_dbgReportCreateInfo.pNext		= nullptr;
    m_dbgReportCreateInfo.flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                       VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                                       VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                       VK_DEBUG_REPORT_DEBUG_BIT_EXT;

    // Create the debug report callback and store the handle into 'debugReportCallback'
    result = m_dbgCreateDebugReportCallback(instance, &m_dbgReportCreateInfo, nullptr, & m_debugReportCallback);
    if (result == VK_SUCCESS) {
        std::cout << "Debug report callback object created successfully\n";
    }
    return result;


    return VK_ERROR_INITIALIZATION_FAILED;
}

void LayerAndExtension::destroyDebugReportCallback()
{
    auto* appObj = VulkanApplication::getInstance();
    auto& instance = appObj->m_instanceObj;
    m_dbgDestroyDebugReportCallback(instance, m_debugReportCallback, nullptr);
}