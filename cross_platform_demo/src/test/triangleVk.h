//
// Created by william on 2021/5/18.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
#define CROSS_PLATFORM_DEMO_TRIANGLE_VK_H

#include "core/vk/commonHandle_vk.h"
#include "core/vk/instance.h"

namespace graphicEngine::vk
{
class TriangleVk
{
private:
    /**
     * @brief 交换链状态机
     */
    struct SwapChainDimension
    {
        /// 交换链的宽
        uint32_t width = 0;
        /// 交换链的高
        uint32_t height = 0;
        /// 交换链的像素格式
        VkFormat format = VK_FORMAT_UNDEFINED;
    };

    /**
     * brief 每一帧的数据
     */
    struct PerFrame
    {
        /// 逻辑设备通过VkDevice句柄来表示, 一个逻辑设备被创建为与一个物理设备的连接
        VkDevice* device = VK_NULL_HANDLE;
        ///     VkFence是一种同步基单元，可用于从队列向主机插入一个依赖关系，VkFence有两种状态--有信号的和无信号的，
        /// VkFence可以作为执行队列提交命令的一部分被信号化。VkFence可以通过 vkResetFences 在主机上取消信号
        /// 主机可以用vkWaitForFences命令等待Fences，并且可以用vkGetFenceStatus查询当前状态。
        ///     VkFence的内部数据可能包括对任何资源的引用，以及与在该fence对象上执行的信号或非信号操作相关的待办工作，
        /// 统称为fence的有效载荷。下面提供了将该内部数据导入和导出围栏的机制。这些机制间接地使应用程序能够在两个或多个
        /// fence之间共享fence状态和其他跨进程和API边界的同步基元。
        VkFence queueSubmitFence = VK_NULL_HANDLE;
        ///     Command pools是不透明的对象，命令缓冲区的内存是由它分配的，它允许实现在多个命令缓冲区中摊销资源创建的成本。
        /// 命令池是外部同步的,这意味着一个命令池不能在多个线程中同时使用。这包括通过记录命令对从池中分配的任何命令缓冲区的
        /// 使用，以及分配、释放和重置命令缓冲区或池本身的操作。
        VkCommandPool primaryCommandPool = VK_NULL_HANDLE;
        ///     命令缓冲区:命令缓冲区是用来记录命令的对象,这些命令随后可以提交到设备队列中执行.命令缓冲区有两个层次--
        /// 一级命令缓冲区,它可以执行二级命令缓冲区,并提交给队列；二级命令缓冲区，它可以由一级命令缓冲区执行，但不直接
        /// 提交给队列。
        ///     命令缓冲区记录的命令包括将管道和描述符集绑定到命令缓冲区的命令,修改动态状态的命令,绘制的命令（用于图形渲染）,
        /// 调度的命令（用于计算）,执行二级命令缓冲区的命令（仅用于一级命令缓冲区）,复制缓冲区和图像的命令，以及其他命令。
        ///     每个命令缓冲区独立于其他命令缓冲区管理状态,在主要和次要命令缓冲区之间或次要命令缓冲区之间不存在状态的继承性
        /// ,当一个命令缓冲区开始记录时,该命令缓冲区的所有状态是未定义的。当二级命令缓冲区被记录到主命令缓冲区上执行时，二级
        /// 命令缓冲区不继承主命令缓冲区的状态，在记录执行二级命令缓冲区命令后，主命令缓冲区的所有状态都是未定义的。这条规则
        /// 有一个例外--如果主命令缓冲区在一个渲染通道实例内，那么渲染通道和子通道的状态就不会被执行二级命令缓冲区所干扰
        /// 。对于依赖状态的命令（如绘制和调度），这些命令所消耗的任何状态都不能是未定义的。
        ///    命令缓冲区的生命周期:
        /// 每个命令缓冲区总是处于以下状态之一:
        /// 初始状态:
        /// 当一个命令缓冲区被分配时，它处于初始状态。一些命令能够将一个命令缓冲区（或一组命令缓冲区）从可执行、记录或无效状态
        /// 中的任何一个重置到这个状态。初始状态的命令缓冲区只能被移动到记录状态，或者被释放。
        /// 记录状态:
        /// vkBeginCommandBuffer将命令缓冲区的状态从初始状态变为记录状态。一旦命令缓冲区处于记录状态，就可以使用vkCmd*命令来记录到命令缓冲区。
        /// 可执行:
        /// vkEndCommandBuffer结束命令缓冲区的记录，并将其从记录状态移到可执行状态。可执行的命令缓冲区可以被提交、重置或记录到另一个命令缓冲区。
        /// 待定:Pending
        /// 命令缓冲区的队列提交将命令缓冲区的状态从可执行状态变为待定状态。在等待状态下，应用程序不得以任何方式试图修改命令缓冲区--因为设备可能正
        /// 在处理记录到它的命令。一旦命令缓冲区的执行完成，命令缓冲区或者恢复到可执行状态，或者如果它是用VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        /// 记录的，它将移动到无效状态。应使用同步命令来检测何时发生这种情况。
        /// 无效状态:
        /// 一些操作，如修改或删除记录在命令缓冲区中的资源，将使该命令缓冲区的状态过渡到无效状态。无效状态的命令缓冲区只能被重置或释放。
        ///     重置命令缓冲区是一种操作，它抛弃了以前记录的任何命令，并将命令缓冲区置于初始状态。重置是作为vkResetCommandBuffer或
        /// vkResetCommandPool的结果发生的，或者作为vkBeginCommandBuffer的一部分（它另外将命令缓冲区置于记录状态）。
        ///     二级命令缓冲区可以通过vkCmdExecuteCommands记录到一级命令缓冲区。这部分地将两个命令缓冲区的生命周期联系在一起--如果主命令被提交到队列中，
        /// 主命令和任何记录到它的次命令都会移动到待定状态。一旦主命令的执行完成，记录在其中的任何次要命令也会完成。
        /// 在每个命令缓冲区的所有执行完成后，它们各自移动到其适当的完成状态（如上所述，移动到可执行状态或无效状态）。
        ///     如果一个次要的移动到无效状态或初始状态，那么它所记录的所有主要缓冲区都移动到无效状态。一个主缓冲区移动到任何其他状态并
        /// 不影响记录在其中的一个次级缓冲区的状态。
        VkCommandBuffer primaryCommandBuffer = VK_NULL_HANDLE;
        ///     Semaphores是一种同步原语,可以用来在队列操作之间或队列操作与主机之间插入一种依赖关系,二进制信号有两种状态--有信号和无信号。时间线
        /// 信号有一个严格增加的64位无符号整数有效载荷,并就一个特定的参考值发出信号。在一个队列操作的执行完成后，一个信号灯可以被发出信号，
        /// 一个队列操作可以在开始执行前等待信号灯成为信号灯。一个时间线信号可以通过vkSignalSemaphore命令从主机发出信号，并通过
        /// vkWaitSemaphores命令从主机等待。
        ///     一个semaphore的内部数据可以包括对任何资源的引用，以及与在该semaphore对象上执行的信号或非信号操作相关的待办工作，
        /// 统称为该semaphore的有效载荷。下面提供了将内部数据导入和导出信号机的机制。这些机制间接地使应用程序能够在两个或多个semaphores之间共享semaphore状态，
        /// 以及跨越进程和API边界的其他同步原语。
        VkSemaphore swapChainAcquireSemaphore = VK_NULL_HANDLE;

        VkSemaphore swapChainReleaseSemaphore = VK_NULL_HANDLE;

        int32_t queueIndex;
    };

    /**
     * @brief Vulkan对象的全局状态。
     */
    struct Context
    {
        /// Vulkan实例.
        VkInstance instance = VK_NULL_HANDLE;
        /// Vulkan物理设备.
        VkPhysicalDevice gpu = VK_NULL_HANDLE;
        /// Vulkan逻辑设备.
        VkDevice device = VK_NULL_HANDLE;
        /// Vulkan逻辑设备队列：创建一个逻辑设备也会创建与该设备相关的队列。要创建的队列由一组VkDeviceQueueCreateInfo结构描述，
        /// 这些结构在pQueueCreateInfos中传递给vkCreateDevice。
        VkQueue queue = VK_NULL_HANDLE;
        /// 交换链
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        /// 交换链规格
        SwapChainDimension swapChainDimensions{};
        /// 即将渲染的surface,Vulkan原生平台的表面或窗口对象被表面对象抽象出来，表面对象用VkSurfaceKHR句柄表示, layer 可以包裹 VkSurfaceKHR对象。
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        /// The queue family index where graphics work will be submitted.(将提交图形工作的队列家族索引)
        int32_t graphicsQueueIndex = -1;
        /// The image view for each swapchain image.
        std::vector<VkImageView> swapchainImageViews;
        /// The renderpass description.
        VkRenderPass renderPass = VK_NULL_HANDLE;
        /// The graphics pipeline.
        VkPipeline pipeline = VK_NULL_HANDLE;
        /**
         * The pipeline layout for resources.
         * Not used in this sample, but we still need to provide a dummy one.
         */
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        /// The debug report callback.
        VkDebugReportCallbackEXT debugCallback = VK_NULL_HANDLE;
        /// A set of semaphores that can be reused.
        std::vector<VkSemaphore> recycledEmaphores;
        /// A set of per-frame data.
        std::vector<PerFrame> perFrame;
    };

public:
    TriangleVk();
    virtual ~TriangleVk();
    void run();

private:
    void teardown(Context& context);
    void createInstance();
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

private:
    Context m_context;
    std::unique_ptr<Instance> m_vulkanInstance;
};
}

#endif //CROSS_PLATFORM_DEMO_TRIANGLE_VK_H
