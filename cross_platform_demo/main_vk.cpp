//#include "base.h"
//#include "logger.h"
//#include "triangleVk.h"
//
//using namespace graphicEngine::vk;
//
//int main()
//{
//    TriangleVk triangle;
//    return 0;
//}

///// Staging Buffer
//// 由于硬件限制，很多时候提供一片同时能被 GPU 和 CPU 读写的内存（both device local and host visible）不是一件容易的事情，
//// 这尤其体现在与独立显卡的交互过程中：CPU 不得不在背后将所有数据从 PCI-E 总线发到 GPU 端，这比起直接访问显卡设备的 VRAM 要慢很多。
//// 为了优化这一点，我们开辟一块和原始顶点内容相等的临时缓冲，将其内存设为共享（和旧式顶点缓冲一样），成为 staging buffer。
//// 在应用了暂存缓冲后，顶点缓冲则可以成为一个使用 GPU 独占显存的缓冲区，GPU对其的读取速度要比从共享内存快
//// 操作从暂存缓冲移动到高速的顶点缓冲，之后每个render loop显卡将都从高速顶点缓冲中取数据进行绘制，在顶点数据不需要改变的情况下（比如场景中的大部分物体）增加效率。
////
//
///// 简单来说，一个RenderPass就是渲染管线的单次运行,一个RenderPass将图像渲染到内存中的帧缓冲附件中。在渲染过程开始时，每个附件需要在块状内存中初始化而且在渲染结束时可能需要写回到内存中。
///// 1、通俗说法:一系列DrawCall的集合, 偏底层一些的说法：一次FBO的bind以及后面的所有绘制命令还有最后的Unbind这个FBO
//
///// subpass和renderpass都是走一个pipeline（VS->TS->GS->光栅化->FS），只不过多个subpass可以通过input attchment利用前一个subpass的output结果，直接访问tiled bufferd的同一处，
/// 不需要访问内存，而多个renderpass如果要利用前一个renderpass的结果，则需要利用纹理采样的方式访问内存。
#define GLFW_INCLUDE_VULKAN
#include "base.h"

#define DEMO_TEXTURE_COUNT 1
#define VERTEX_BUFFER_BIND_ID 0
#define APP_SHORT_NAME "triangle"
#define APP_LONG_NAME "The Vulkan Triangle Demo Program"

/// 告诉编译器忽略此告警
#if defined(NDEBUG) && defined(__GNUC__)
    #define U_ASSERT_ONLY __attribute__((unused))
#else
    #define U_ASSERT_ONLY
#endif

// 纹理对象
struct textureObject
{
    VkSampler sampler;           // 纹理资源：(GPU)显存,VkSampler对象表示图像采样器的状态，实现使用它来读取图像数据并为着色器应用过滤和其他转换
    VkImage image;               // 图像资源：内存,VkImage代表了多维的(最多3个)数据数组,可以用于各种目的（例如附件、纹理）通过描述符集将它们绑定到图形或计算管道上，或者直接将它们作为参数指定给某些命令。
    VkImageLayout imageLayout;   // 图像布局: 是一个枚举
    VkDeviceMemory mem;          // 设备内存对象：Vulkan设备通过在 API中由VkDeviceMemory句柄表示的内存对象对设备内存中的数据进行操作。
    VkImageView view;            // 图像视图:图像对象不能直接通过pipeline直接读写图像数据。相反，表示图像子资源的连续范围并包含附加元数据的图像视图用于该目的。视图必须在兼容类型的图像上创建，并且必须代表图像子资源的有效子集。
    int32_t texWidth, texHeight; // 纹理宽，高
};

VKAPI_ATTR VkBool32 VKAPI_CALL
    BreakCallback(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
                  uint64_t srcObject, size_t location, int32_t msgCode,
                  const char* pLayerPrefix, const char* pMsg,
                  void* pUserData)
{
    raise(SIGTRAP);
    return false;
}

/// 交换链:是一系列虚拟帧缓冲区,由显卡和图形API用于稳定帧速率和其他一些功能。swapchain通常存在于图形内存中，但也可以存在于系统内存中,
/// 不使用swapchain可能会导致渲染卡顿，它的存在和使用是许多图形api所必需的，具有两个缓冲区的swapchain是双缓冲区。
/// 功能:
/// 每个swapchain中至少有两个缓冲区,
/// 第一个framebuffer，即screenbuffer(有时也称为front buffer)，是呈现到视频卡输出的缓冲区,
/// 其余的缓冲区称为后向缓冲区(back buffer)。每次显示一个新帧时，交换链中的第一个backbuffer将取代screenbuffer，这称为“呈现(present)”或"交换(swap)"。
typedef struct
{
    VkImage image;       // 图像数据数组
    VkCommandBuffer cmd; // 命令缓冲区:
    VkImageView view;    // 图像视图
} SwapChainBuffers;

struct Demo
{
    GLFWwindow* window;                       /// glfw窗口
    VkSurfaceKHR surface;                     /// Vulkan原生平台的表面或窗口对象被表面对象抽象出来，表面对象用VkSurfaceKHR句柄表示, layer 可以包裹 VkSurfaceKHR对象。
    VkInstance instance;                      /// vk实例,用于创建各种资源,比如逻辑device
    VkDebugReportCallbackEXT msgCallback;     /// debug层扩展
    VkPhysicalDevice gpu;                     /// 物理设备
    VkDevice device;                          /// 逻辑设备
    VkQueue queue;                            /// 队列
    VkPhysicalDeviceProperties gpuProperties; /// 物理设备属性
    VkPhysicalDeviceFeatures gpuFeatures;     /// 物理设备特性
    VkQueueFamilyProperties* queueProperties; /// 队列family属性
    uint32_t graphicsQueueNodeIndex;
    uint32_t enabledExtensionCount;
    uint32_t enabledLayerCount;
    const char* extensionNames[64]; /// 扩展名
    const char* enabledLayers[64];  /// 扩展层

    int width, height;
    VkFormat format;            /// 是一个枚举,服务于colorSpace
    VkColorSpaceKHR colorSpace; /// 颜色空间，是一个枚举，表示引擎支持的颜色空间，
    /// 虽然format可呈现图像的 是指每个像素的编码，但它colorSpace决定了呈现引擎如何解释像素值。
    /// 本文档中的色彩空间是指特定色彩空间（由其原色的色度和 CIE Lab 中的白点定义），以及在给定色彩空间中存储或传输色彩数据之前应用的传递函数。
    uint32_t swapChainImageCount;
    VkSwapchainKHR swapChain;           /// vk交换链
    SwapChainBuffers* swapChainBuffers; /// vk交换链缓冲

    VkCommandPool cmdPool; /// vk指令池

    struct
    {
        VkFormat format;    /// 深度格式
        VkImage image;      /// (CPU)图像内存
        VkDeviceMemory mem; /// 设备内存
        VkImageView view;   /// 图像句柄
    } depth;                /// 深度

    struct textureObject textures[DEMO_TEXTURE_COUNT]; //纹理

    struct
    {
        VkBuffer buf;                                              /// 顶点缓冲
        VkDeviceMemory mem;                                        /// 设备内存
        VkPipelineVertexInputStateCreateInfo vertexInputCreatInfo; /// 顶点管线输入状态信息，包括类型，顶点输入Binding描述符，顶点输入Binding描述符的个数，顶点输入属性描述符，顶点输入属性描述符的个数
        VkVertexInputBindingDescription vertexInputBindings[1];    /// 顶点输入Binding描述符：包括binding，stride，inputRate（顶点和instance）
        VkVertexInputAttributeDescription vertexInputAttrs[2];     /// 顶点输入属性描述符，包括location，binding， offset，format
    } vertices;                                                    /// 顶点

    VkCommandBuffer setupCmd;         // Command Buffer for initialization commands
    VkCommandBuffer drawCmd;          // Command Buffer for drawing commands
    VkPipelineLayout pipelineLayout;  /// 管线布局：这个布局序列被用来确定着色器阶段和着色器资源之间的接口。每条流水线都是用流水线布局创建的。
    VkDescriptorSetLayout descLayout; /// Descriptor设置布局
    VkPipelineCache pipelineCache;    /// 管线缓存
    VkRenderPass renderPass;          /// 渲染通道
    /// 渲染通道代表了attachments、subpasses和subpasses之间的依赖关系的集合，并描述了附件在subpasses过程中的使用方式。渲染通道在命令缓冲区中的使用是一个渲染通道实例。
    /// attachment:描述描述了一个attachment的属性，包括它的格式、采样数，以及在每个渲染通道实例的开始和结束时如何处理其内容。
    /// subpass：渲染的一个阶段，它是在一个渲染通道中读写附件的一个子集。渲染命令是被记录到一个渲染通道实例的特定子通道中。
    /// 一个subpass描述了参与执行一个subpass的附件子集,每个subpass可以从一些attachment中读取输入附件，
    /// 将一些附件写成color_attachments,或depth/stencil_attachments，对color_attachments或depth/stencil_attachments执行着色解析操作，
    /// 并对解析附件执行多采样解析操作。一个subpass的描述还可以包括一组preserve attachments，这些附件不被子通读取或写入，但其内容必须在整个子通中被保留下来。
    /// 如果attachment是该subpass的color,depth/stencil,resolve,depth/stencil resolve,fragment shading rate, or input attachment for that subpass,
    /// 由pColorAttachments、pDepthStencilAttachment、pResolveAttachments、VkSubpassDescriptionDepthStencilResolve::pDepthStencilResolveAttachment、
    /// VkFragmentShadingRateAttachmentInfoKHR::pFragmentShadingRateAttachment->attachment，以及VkSubpassDescription的pInputAttachments成员
    /// 如果一个subpass被另一个subpass保留了attachment，那么该attachment就不会被使用。一个attachment第一次使用是在使用该attachment的最低编号的subpass中。
    /// 同样地，一个attachment的最后一次使用是在使用该attachment的最高编号的subpass中。
    /// 一个renderpass 中的subpass都渲染成相同的尺寸，一个subpass中的像素（x,y,layer）的片段只能读取以前的子通道在相同的（x,y,layer）位置上写的Attachments内容。
    /// 对于多像素片段，从输入附件中读取的像素是以一种依赖于实现的方式从该片段所覆盖的像素中选择的。然而，这种选择必须在VkDevice的生命周期内对任何具有相同着色率的片段做出一致的选择。
    /// pResolveAttachments:解析附件

    VkPipeline pipeline; /// 渲染管线

    VkShaderModule vertShaderModule; /// 顶点着色器
    VkShaderModule fragShaderModule; /// 片元着色器

    VkDescriptorPool descPool; /// 描述符池
    VkDescriptorSet descSet;   /// 描述符Set

    VkFramebuffer* frameBuffers; /// 帧缓冲
    /// renderpass与frameBuffer一起运作。frameBuffer代表了renderpass实例所使用的特定内存附件的集合。

    VkPhysicalDeviceMemoryProperties memoryProperties; /// 物理设备内存属性

    int32_t curFrame; ///当前帧
    int32_t frameCount;
    bool validate;
    bool use_break;
    float depthStencil;
    float depthIncrement;

    uint32_t currentBuffer;
    uint32_t queueCount;
    bool useStagingBuffer;
};

VKAPI_ATTR VkBool32 VKAPI_CALL
    dbgFunc(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
            uint64_t srcObject, size_t location, int32_t msgCode,
            const char* pLayerPrefix, const char* pMsg, void* pUserData)
{
    char* message = (char*)malloc(strlen(pMsg) + 100);

    assert(message);

    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        sprintf(message, "ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode,
                pMsg);
    }
    else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        sprintf(message, "WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode,
                pMsg);
    }
    else
    {
        return false;
    }

    printf("%s\n", message);
    fflush(stdout);
    free(message);

    /*
    * false indicates that layer should not bail-out of an
    * API call that had validation failures. This may mean that the
    * app dies inside the driver due to invalid parameter(s).
    * That's what would happen without validation layers, so we'll
    * keep that behavior here.
    */
    return false;
}

// Forward declaration:
static void demoResize(struct Demo* demo);

static bool memoryTypeFromProperties(struct Demo* demo, uint32_t typeBits,
                                     VkFlags requirements_mask,
                                     uint32_t* typeIndex)
{
    uint32_t i;
    // Search memtypes to find first index with those properties
    for (i = 0; i < VK_MAX_MEMORY_TYPES; i++)
    {
        if ((typeBits & 1) == 1)
        {
            // Type is available, does it match user properties?
            if ((demo->memoryProperties.memoryTypes[i].propertyFlags &
                 requirements_mask) == requirements_mask)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    // No memory types matched, return failure
    return false;
}

static void demoFlushInitCmd(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;

    if (demo->setupCmd == VK_NULL_HANDLE)
        return;

    err = vkEndCommandBuffer(demo->setupCmd); /// 来表明你已经完成了，并且把 command buffer 退出了“recording”状态，使它准备好了被使用。
    assert(!err);

    const VkCommandBuffer cmd_bufs[] = { demo->setupCmd };
    VkFence nullFence = { VK_NULL_HANDLE };
    VkSubmitInfo submit_info = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                 .pNext = nullptr,
                                 .waitSemaphoreCount = 0,
                                 .pWaitSemaphores = nullptr,
                                 .pWaitDstStageMask = nullptr,
                                 .commandBufferCount = 1,
                                 .pCommandBuffers = cmd_bufs,
                                 .signalSemaphoreCount = 0,
                                 .pSignalSemaphores = nullptr };
    /// sType 是这种结构的类型。
    /// pNext是NULL或指向扩展此结构的结构的指针。
    /// flags是VkSubmitFlagBitsKHR的位掩码。
    /// waitSemaphoreInfoCount是 中的元素数 pWaitSemaphoreInfos。
    /// pWaitSemaphoreInfos是一个指向定义信号量等待操作的VkSemaphoreSubmitInfoKHR结构 数组的指针 。
    /// commandBufferInfoCount是pCommandBufferInfos要在批处理中执行的元素 数和命令缓冲区数。
    /// pCommandBufferInfos是一个指向VkCommandBufferSubmitInfoKHR结构数组的指针，该 结构描述要在批处理中执行的命令缓冲区。
    /// signalSemaphoreInfoCount是 中的元素数 pSignalSemaphoreInfos。
    /// pSignalSemaphoreInfos是一个指向描述 信号量信号操作的VkSemaphoreSubmitInfoKHR数组的指针 。

    err = vkQueueSubmit(demo->queue, 1, &submit_info, nullFence);
    /// vkQueueSubmit:表示要将命令缓冲区提交到队列
    /// queue 是命令缓冲区将提交到的队列。
    /// submitCount是pSubmits数组中元素的数量。
    /// pSubmits是一个指向VkSubmitInfo 结构数组的指针，每个结构指定一个命令缓冲区提交批处理。
    /// fence是一个栅栏的可选句柄，一旦所有提交的命令缓冲区完成执行，就会发出信号,如果fence不是VK_NULL_HANDLE，则定义一个 栅栏信号操作。

    assert(!err);

    err = vkQueueWaitIdle(demo->queue); // 等待闲置
    assert(!err);

    vkFreeCommandBuffers(demo->device, demo->cmdPool, 1, cmd_bufs);
    demo->setupCmd = VK_NULL_HANDLE;
}

static void demoSetImageLayout(struct Demo* demo, VkImage image,
                               VkImageAspectFlags aspectMask,
                               VkImageLayout old_image_layout,
                               VkImageLayout new_image_layout,
                               VkAccessFlagBits srcAccessMask)
{
    VkResult U_ASSERT_ONLY err;

    if (demo->setupCmd == VK_NULL_HANDLE)
    {
        const VkCommandBufferAllocateInfo cmd = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = demo->cmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        err = vkAllocateCommandBuffers(demo->device, &cmd, &demo->setupCmd);
        assert(!err);

        VkCommandBufferBeginInfo cmd_buf_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr,
        };
        err = vkBeginCommandBuffer(demo->setupCmd, &cmd_buf_info);
        ///  要开始记录命令缓冲区:
        /// commandBuffer 是要进入记录状态的命令缓冲区的句柄。
        /// pBeginInfo是一个指向VkCommandBufferBeginInfo 结构的指针，该结构定义了有关命令缓冲区如何开始记录的附加信息。
        assert(!err);
    }
    /// VkImageMemoryBarrier记录：srcAccessMask, dstAccessMask, oldLayout, newLayout, srcQueueFamilyIndex, dstQueueFamilyIndex, image,subresourceRange
    VkImageMemoryBarrier image_memory_barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = srcAccessMask,
        .dstAccessMask = 0,
        .oldLayout = old_image_layout,
        .newLayout = new_image_layout,
        .image = image,
        .subresourceRange = { aspectMask, 0, 1, 0, 1 }
    };

    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        /* Make sure anything that was copying from this image has completed */
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
    {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        /* Make sure any Copy or CPU writes to image are flushed */
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    }

    VkImageMemoryBarrier* pmemory_barrier = &image_memory_barrier;

    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    /// Vulkan的barrier允许我们在同一队列，同一子流程插入依赖, 执行依赖通过我们指定的pipeline stage同步作用域指定。
    /// 除了执行依赖，vkCmdPipelineBarrier可以插入3种存取依赖：全局，缓冲和图像。使用memory barrier，我们可以保证
    /// 第一同步作用域的写入操作会在第二同步作用域的读取操作前完成。我们可以使用全局memory barrier同步所有存取访问，
    /// 而不仅仅是一个特定的资源。进行细粒度的同步，可以使用buffer memory和image memory barrier。
    vkCmdPipelineBarrier(demo->setupCmd, src_stages, dest_stages, 0, 0, nullptr,
                         0, nullptr, 1, pmemory_barrier);
}

static void demo_draw_build_cmd(struct Demo* demo)
{
    const VkCommandBufferBeginInfo cmd_buf_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    /// sType 是这种结构的类型。
    /// pNext是NULL或指向扩展此结构的结构的指针。
    /// flags是VkCommandBufferUsageFlagBits的位掩码， 指定命令缓冲区的使用行为。
    /// pInheritanceInfo是指向VkCommandBufferInheritanceInfo结构的指针 ，如果 commandBuffer是辅助命令缓冲区则使用。如果这是主命令缓冲区，则忽略此值

    const VkClearValue clearValues[2] = {
        { .color.float32 = { 0.2f, 0.2f, 0.2f, 0.2f } },
        { .depthStencil = { demo->depthStencil, 0 } },
    };
    const VkRenderPassBeginInfo rpBegin = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = demo->renderPass,
        .framebuffer = demo->frameBuffers[demo->currentBuffer],
        .renderArea.offset.x = 0,
        .renderArea.offset.y = 0,
        .renderArea.extent.width = static_cast<uint32_t>(demo->width),
        .renderArea.extent.height = static_cast<uint32_t>(demo->height),
        .clearValueCount = 2,
        .pClearValues = clearValues,
    };
    /// sType 是这种结构的类型。
    /// pNext是NULL或指向扩展此结构的结构的指针。
    /// renderPass 是开始实例的渲染通道。
    /// framebuffer 是包含与渲染通道一起使用的附件的帧缓冲区。
    /// renderArea 是受渲染通道实例影响的渲染区域，下面有更详细的描述。
    /// clearValueCount是 中的元素数pClearValues。
    /// pClearValues是一个指向clearValueCount VkClearValue结构数组的指针，该数组包含每个附件的清晰值，如果附件使用 的loadOp值，
    /// VK_ATTACHMENT_LOAD_OP_CLEAR或者附件具有深度/模板格式并使用stencilLoadOp值 VK_ATTACHMENT_LOAD_OP_CLEAR。该数组按附件编号进行索引。仅使用与清除的附件对应的元素。的其他元素pClearValues被忽略。
    VkResult U_ASSERT_ONLY err;

    err = vkBeginCommandBuffer(demo->drawCmd, &cmd_buf_info);
    assert(!err);

    // We can use LAYOUT_UNDEFINED as a wildcard here because we don't care what
    // happens to the previous contents of the image
    VkImageMemoryBarrier image_memory_barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = demo->swapChainBuffers[demo->currentBuffer].image,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
    };

    vkCmdPipelineBarrier(demo->drawCmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0,
                         nullptr, 1, &image_memory_barrier);
    vkCmdBeginRenderPass(demo->drawCmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
    /// commandBuffer 是用于记录命令的命令缓冲区。
    /// pRenderPassBegin是一个指向VkRenderPassBeginInfo 结构的指针，指定渲染通道开始实例，以及实例使用的帧缓冲区。
    /// contents是一个VkSubpassContents值，指定如何提供第一个subpass中的命令。
    /// 开始渲染通道实例后，命令缓冲区准备好记录该渲染通道的第一个subpass的命令。
    vkCmdBindPipeline(demo->drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      demo->pipeline);
    vkCmdBindDescriptorSets(demo->drawCmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            demo->pipelineLayout, 0, 1, &demo->descSet, 0,
                            nullptr);

    VkViewport viewport;
    memset(&viewport, 0, sizeof(viewport));
    viewport.height = (float)demo->height;
    viewport.width = (float)demo->width;
    viewport.minDepth = (float)0.0f;
    viewport.maxDepth = (float)1.0f;
    vkCmdSetViewport(demo->drawCmd, 0, 1, &viewport);

    VkRect2D scissor;
    memset(&scissor, 0, sizeof(scissor));
    scissor.extent.width = demo->width;
    scissor.extent.height = demo->height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vkCmdSetScissor(demo->drawCmd, 0, 1, &scissor);

    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(demo->drawCmd, VERTEX_BUFFER_BIND_ID, 1,
                           &demo->vertices.buf, offsets);

    vkCmdDraw(demo->drawCmd, 3, 1, 0, 0);
    vkCmdEndRenderPass(demo->drawCmd);
    /// commandBuffer 是结束当前渲染通道实例的命令缓冲区。

    VkImageMemoryBarrier prePresentBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
    };

    prePresentBarrier.image = demo->swapChainBuffers[demo->currentBuffer].image;
    VkImageMemoryBarrier* pmemory_barrier = &prePresentBarrier;
    vkCmdPipelineBarrier(demo->drawCmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0,
                         nullptr, 1, pmemory_barrier);

    err = vkEndCommandBuffer(demo->drawCmd);
    /// 记录开始后，应用程序会记录一系列命令 ( vkCmd*) 以在命令缓冲区中设置状态、绘制、调度和其他命令。也可以从VkBuffer 内容中间接记录一些命令
    /// vkEndCommandBuffer:表示要完成命令缓冲区的记录。
    /// commandBuffer 是完成记录的命令缓冲区。
    /// 如果在录制过程中出现错误，应用程序将收到由 返回的不成功返回码的通知vkEndCommandBuffer。如果应用程序希望进一步使用命令缓冲区，则必须重置命令缓冲区。命令缓冲区必须一直处于记录状态，并移动到可执行状态。
    assert(!err);
}

static void demoDraw(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;
    VkSemaphore imageAcquiredSemaphore, drawCompleteSemaphore;
    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    err = vkCreateSemaphore(demo->device, &semaphoreCreateInfo,
                            nullptr, &imageAcquiredSemaphore);
    assert(!err);

    err = vkCreateSemaphore(demo->device, &semaphoreCreateInfo,
                            nullptr, &drawCompleteSemaphore);
    assert(!err);

    // Get the index of the next available swapChain image:
    err = vkAcquireNextImageKHR(demo->device, demo->swapChain, UINT64_MAX,
                                imageAcquiredSemaphore,
                                (VkFence)VK_NULL_HANDLE, // TODO: Show use of fence
                                &demo->currentBuffer);
    if (err == VK_ERROR_OUT_OF_DATE_KHR)
    {
        // Demo->swapChain is out of date (e.g. the window was resized) and
        // must be recreated:
        demoResize(demo);
        demoDraw(demo);
        vkDestroySemaphore(demo->device, imageAcquiredSemaphore, nullptr);
        vkDestroySemaphore(demo->device, drawCompleteSemaphore, nullptr);
        return;
    }
    else if (err == VK_SUBOPTIMAL_KHR)
    {
        // Demo->swapChain is not as optimal as it could be, but the platform's
        // presentation engine will still present the image correctly.
    }
    else
    {
        assert(!err);
    }

    demoFlushInitCmd(demo);

    // Wait for the present complete semaphore to be signaled to ensure
    // that the image won't be rendered to until the presentation
    // engine has fully released ownership to the application, and it is
    // okay to render to the image.

    demo_draw_build_cmd(demo);
    VkFence nullFence = VK_NULL_HANDLE;
    VkPipelineStageFlags pipe_stage_flags =
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkSubmitInfo submit_info = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                 .pNext = nullptr,
                                 .waitSemaphoreCount = 1,
                                 .pWaitSemaphores = &imageAcquiredSemaphore,
                                 .pWaitDstStageMask = &pipe_stage_flags,
                                 .commandBufferCount = 1,
                                 .pCommandBuffers = &demo->drawCmd,
                                 .signalSemaphoreCount = 1,
                                 .pSignalSemaphores = &drawCompleteSemaphore };
    /// sType 是这种结构的类型。
    /// pNext是NULL或指向扩展此结构的结构的指针。
    /// flags是VkSubmitFlagBitsKHR的位掩码。
    /// waitSemaphoreInfoCount是 中的元素数 pWaitSemaphoreInfos。
    /// pWaitSemaphoreInfos是一个指向定义信号量等待操作的VkSemaphoreSubmitInfoKHR结构 数组的指针 。
    /// commandBufferInfoCount是pCommandBufferInfos要在批处理中执行的元素 数和命令缓冲区数。
    /// pCommandBufferInfos是一个指向VkCommandBufferSubmitInfoKHR结构数组的指针，该 结构描述要在批处理中执行的命令缓冲区。
    /// signalSemaphoreInfoCount是 中的元素数 pSignalSemaphoreInfos。
    /// pSignalSemaphoreInfos是一个指向描述 信号量信号操作的VkSemaphoreSubmitInfoKHR数组的指针 。

    err = vkQueueSubmit(demo->queue, 1, &submit_info, nullFence);
    /// vkQueueSubmit:表示要将命令缓冲区提交到队列
    /// queue 是命令缓冲区将提交到的队列。
    /// submitCount是pSubmits数组中元素的数量。
    /// pSubmits是一个指向VkSubmitInfo 结构数组的指针，每个结构指定一个命令缓冲区提交批处理。
    /// fence是一个栅栏的可选句柄，一旦所有提交的命令缓冲区完成执行，就会发出信号,如果fence不是VK_NULL_HANDLE，则定义一个 栅栏信号操作。

    assert(!err);

    VkPresentInfoKHR present = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &drawCompleteSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &demo->swapChain,
        .pImageIndices = &demo->currentBuffer,
    };

    err = vkQueuePresentKHR(demo->queue, &present);
    if (err == VK_ERROR_OUT_OF_DATE_KHR)
    {
        // Demo->swapChain is out of date (e.g. the window was resized) and
        // must be recreated:
        demoResize(demo);
    }
    else if (err == VK_SUBOPTIMAL_KHR)
    {
        // Demo->swapChain is not as optimal as it could be, but the platform's
        // presentation engine will still present the image correctly.
    }
    else
    {
        assert(!err);
    }

    err = vkQueueWaitIdle(demo->queue);
    assert(err == VK_SUCCESS);

    vkDestroySemaphore(demo->device, imageAcquiredSemaphore, nullptr);
    vkDestroySemaphore(demo->device, drawCompleteSemaphore, nullptr);
}

static void demoPrepareBuffers(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;
    VkSwapchainKHR oldSwapchain = demo->swapChain;

    // Check the surface capabilities and formats
    VkSurfaceCapabilitiesKHR surfCapabilities;
    err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        demo->gpu, demo->surface, &surfCapabilities);
    assert(!err);

    uint32_t presentModeCount;
    err = vkGetPhysicalDeviceSurfacePresentModesKHR(
        demo->gpu, demo->surface, &presentModeCount, nullptr);
    assert(!err);
    auto* presentModes =
        (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    assert(presentModes);
    err = vkGetPhysicalDeviceSurfacePresentModesKHR(
        demo->gpu, demo->surface, &presentModeCount, presentModes);
    assert(!err);

    VkExtent2D swapchainExtent;
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
    {
        // If the surface size is undefined, the size is set to the size
        // of the images requested, which must fit within the minimum and
        // maximum values.
        swapchainExtent.width = demo->width;
        swapchainExtent.height = demo->height;

        if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }

        if (swapchainExtent.height < surfCapabilities.minImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        }
        else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
    }
    else
    {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
        demo->width = surfCapabilities.currentExtent.width;
        demo->height = surfCapabilities.currentExtent.height;
    }

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Determine the number of VkImage's to use in the swap chain.
    // Application desires to only acquire 1 image at a time (which is
    // "surfCapabilities.minImageCount").
    uint32_t desiredNumOfSwapchainImages = surfCapabilities.minImageCount;
    // If maxImageCount is 0, we can ask for as many images as we want;
    // otherwise we're limited to maxImageCount
    if ((surfCapabilities.maxImageCount > 0) &&
        (desiredNumOfSwapchainImages > surfCapabilities.maxImageCount))
    {
        // Application must settle for fewer images than desired:
        desiredNumOfSwapchainImages = surfCapabilities.maxImageCount;
    }

    VkSurfaceTransformFlagsKHR preTransform;
    if (surfCapabilities.supportedTransforms &
        VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = surfCapabilities.currentTransform;
    }

    const VkSwapchainCreateInfoKHR swapchain = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .surface = demo->surface,
        .minImageCount = desiredNumOfSwapchainImages,
        .imageFormat = demo->format,
        .imageColorSpace = demo->colorSpace,
        .imageExtent = {
            .width = swapchainExtent.width,
            .height = swapchainExtent.height,
        },
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform),
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .imageArrayLayers = 1,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .presentMode = swapchainPresentMode,
        .oldSwapchain = oldSwapchain,
        .clipped = true,
    };
    uint32_t i;

    err = vkCreateSwapchainKHR(demo->device, &swapchain, nullptr, &demo->swapChain);
    assert(!err);

    // If we just re-created an existing swapChain, we should destroy the old
    // swapChain at this point.
    // Note: destroying the swapChain also cleans up all its associated
    // presentable images once the platform is done with them.
    if (oldSwapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(demo->device, oldSwapchain, nullptr);
    }

    err = vkGetSwapchainImagesKHR(demo->device, demo->swapChain,
                                  &demo->swapChainImageCount, nullptr);
    assert(!err);

    auto* swapchainImages =
        (VkImage*)malloc(demo->swapChainImageCount * sizeof(VkImage));
    assert(swapchainImages);
    err = vkGetSwapchainImagesKHR(demo->device, demo->swapChain,
                                  &demo->swapChainImageCount,
                                  swapchainImages);
    assert(!err);

    demo->swapChainBuffers = (SwapChainBuffers*)malloc(sizeof(SwapChainBuffers) *
                                                       demo->swapChainImageCount);
    assert(demo->swapChainBuffers);

    for (i = 0; i < demo->swapChainImageCount; i++)
    {
        VkImageViewCreateInfo colorAttachmentView = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .format = demo->format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A,
            },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 },
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .flags = 0,
        };

        demo->swapChainBuffers[i].image = swapchainImages[i];

        colorAttachmentView.image = demo->swapChainBuffers[i].image;

        err = vkCreateImageView(demo->device, &colorAttachmentView, nullptr,
                                &demo->swapChainBuffers[i].view);
        /// device 是创建映像的逻辑设备。
        /// pCreateInfo是指向包含用于创建图像的参数的VkImageCreateInfo结构的指针。
        /// pAllocator控制主机内存分配，如 内存分配章节所述。
        /// pImage是一个指向VkImage句柄的指针，在该句柄中返回结果图像对象。
        assert(!err);
    }

    demo->currentBuffer = 0;

    if (presentModes != nullptr)
    {
        free(presentModes);
    }
}

static void demo_prepare_depth(struct Demo* demo)
{
    const VkFormat depthFormat = VK_FORMAT_D16_UNORM;
    const VkImageCreateInfo image = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depthFormat,
        .extent = { static_cast<uint32_t>(demo->width), static_cast<uint32_t>(demo->height), 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .flags = 0,
    };
    VkMemoryAllocateInfo memAlloc = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = 0,
        .memoryTypeIndex = 0,
    };
    VkImageViewCreateInfo view = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .image = VK_NULL_HANDLE,
        .format = depthFormat,
        .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                              .baseMipLevel = 0,
                              .levelCount = 1,
                              .baseArrayLayer = 0,
                              .layerCount = 1 },
        .flags = 0,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
    };
    /// VkMemoryRequirements包括：size， alignment，memoryTypeBits
    VkMemoryRequirements memReqs;
    VkResult U_ASSERT_ONLY err;
    bool U_ASSERT_ONLY pass;

    demo->depth.format = depthFormat;

    /* create image */
    err = vkCreateImage(demo->device, &image, nullptr, &demo->depth.image);
    assert(!err);

    /* get memory requirements for this object */
    vkGetImageMemoryRequirements(demo->device, demo->depth.image, &memReqs);

    /* select memory size and type */
    memAlloc.allocationSize = memReqs.size;
    pass = memoryTypeFromProperties(demo, memReqs.memoryTypeBits,
                                    0, /* No requirements */
                                    &memAlloc.memoryTypeIndex);
    assert(pass);

    /* allocate memory */
    err = vkAllocateMemory(demo->device, &memAlloc, nullptr, &demo->depth.mem);
    /// device 是拥有内存的逻辑设备。
    /// pAllocateInfo是一个指向 描述分配参数的VkMemoryAllocateInfo结构的指针。成功返回的分配必须使用请求的参数——实现不允许替换。
    /// pAllocator控制主机内存分配。
    /// pMemory是一个指向VkDeviceMemory句柄的指针，其中返回有关已分配内存的信息。

    assert(!err);

    /* bind memory */
    err =
        vkBindImageMemory(demo->device, demo->depth.image, demo->depth.mem, 0);
    assert(!err);

    demoSetImageLayout(demo, demo->depth.image, VK_IMAGE_ASPECT_DEPTH_BIT,
                       VK_IMAGE_LAYOUT_UNDEFINED,
                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                       static_cast<VkAccessFlagBits>(0));

    /* create image view */
    view.image = demo->depth.image;
    err = vkCreateImageView(demo->device, &view, nullptr, &demo->depth.view);
    assert(!err);
}

static void
    demoPrepareTextureImage(struct Demo* demo, const uint32_t* texColors,
                            struct textureObject* texObj, VkImageTiling tiling,
                            VkImageUsageFlags usage, VkFlags requiredProps)
{
    const VkFormat texFormat = VK_FORMAT_B8G8R8A8_UNORM;
    const int32_t texWidth = 2;
    const int32_t texHeight = 2;
    VkResult U_ASSERT_ONLY err;
    bool U_ASSERT_ONLY pass;

    texObj->texWidth = texWidth;
    texObj->texHeight = texHeight;

    const VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = texFormat,
        .extent = { texWidth, texHeight, 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = tiling,
        .usage = usage,
        .flags = 0,
        .initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED
    };
    VkMemoryAllocateInfo mem_alloc = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = 0,
        .memoryTypeIndex = 0,
    };

    VkMemoryRequirements memReqs;

    err = vkCreateImage(demo->device, &imageCreateInfo, nullptr, &texObj->image);
    assert(!err);

    vkGetImageMemoryRequirements(demo->device, texObj->image, &memReqs);

    mem_alloc.allocationSize = memReqs.size;
    pass = memoryTypeFromProperties(demo, memReqs.memoryTypeBits,
                                    requiredProps, &mem_alloc.memoryTypeIndex);
    assert(pass);

    /* allocate memory */
    err = vkAllocateMemory(demo->device, &mem_alloc, nullptr, &texObj->mem);
    assert(!err);

    /* bind memory */
    err = vkBindImageMemory(demo->device, texObj->image, texObj->mem, 0);
    assert(!err);

    if (requiredProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        const VkImageSubresource subres = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .arrayLayer = 0,
        };
        VkSubresourceLayout layout;
        void* data;
        int32_t x, y;

        vkGetImageSubresourceLayout(demo->device, texObj->image, &subres,
                                    &layout);

        err = vkMapMemory(demo->device, texObj->mem, 0,
                          mem_alloc.allocationSize, 0, &data);
        assert(!err);

        for (y = 0; y < texHeight; ++y)
        {
            auto* row = (uint32_t*)((char*)data + layout.rowPitch * y);
            for (x = 0; x < texWidth; ++x)
                row[x] = texColors[(x & 1) ^ (y & 1)];
        }

        vkUnmapMemory(demo->device, texObj->mem);
    }

    texObj->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    demoSetImageLayout(demo, texObj->image, VK_IMAGE_ASPECT_COLOR_BIT,
                       VK_IMAGE_LAYOUT_PREINITIALIZED, texObj->imageLayout,
                       VK_ACCESS_HOST_WRITE_BIT);
    /* setting the image layout does not reference the actual memory so no need
     * to add a mem ref */
}

static void demoDestroyTextureImage(struct Demo* demo,
                                    struct textureObject* texObj)
{
    /* clean up staging resources */
    vkDestroyImage(demo->device, texObj->image, nullptr);
    vkFreeMemory(demo->device, texObj->mem, nullptr);
}

static void demoPrepareTextures(struct Demo* demo)
{
    const VkFormat texFormat = VK_FORMAT_B8G8R8A8_UNORM;
    VkFormatProperties props;
    const uint32_t texColors[DEMO_TEXTURE_COUNT][2] = {
        { 0xffff0000, 0xff00ff00 },
    };
    VkResult U_ASSERT_ONLY err;

    vkGetPhysicalDeviceFormatProperties(demo->gpu, texFormat, &props);

    for (uint32_t i = 0; i < DEMO_TEXTURE_COUNT; ++i)
    {
        if ((props.linearTilingFeatures &
             VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
            !demo->useStagingBuffer)
        {
            /* Device can texture using linear textures */
            demoPrepareTextureImage(
                demo, texColors[i], &demo->textures[i], VK_IMAGE_TILING_LINEAR,
                VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        }
        else if (props.optimalTilingFeatures &
                 VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
        {
            /* Must use staging buffer to copy linear texture to optimized */
            struct textureObject stagingTexture{};

            memset(&stagingTexture, 0, sizeof(stagingTexture));
            demoPrepareTextureImage(
                demo, texColors[i], &stagingTexture, VK_IMAGE_TILING_LINEAR,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            demoPrepareTextureImage(
                demo, texColors[i], &demo->textures[i],
                VK_IMAGE_TILING_OPTIMAL,
                (VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            demoSetImageLayout(demo, stagingTexture.image,
                               VK_IMAGE_ASPECT_COLOR_BIT,
                               stagingTexture.imageLayout,
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               static_cast<VkAccessFlagBits>(0));

            demoSetImageLayout(demo, demo->textures[i].image,
                               VK_IMAGE_ASPECT_COLOR_BIT,
                               demo->textures[i].imageLayout,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               static_cast<VkAccessFlagBits>(0));

            VkImageCopy copyRegion = {
                .srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
                .srcOffset = { 0, 0, 0 },
                .dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
                .dstOffset = { 0, 0, 0 },
                .extent = { static_cast<uint32_t>(stagingTexture.texWidth),
                            static_cast<uint32_t>(stagingTexture.texHeight), 1 },
            };
            vkCmdCopyImage(
                demo->setupCmd, stagingTexture.image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, demo->textures[i].image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

            demoSetImageLayout(demo, demo->textures[i].image,
                               VK_IMAGE_ASPECT_COLOR_BIT,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               demo->textures[i].imageLayout,
                               static_cast<VkAccessFlagBits>(0));

            demoFlushInitCmd(demo);

            demoDestroyTextureImage(demo, &stagingTexture);
        }
        else
        {
            /* Can't support VK_FORMAT_B8G8R8A8_UNORM !? */
            assert(!"No support for B8G8R8A8_UNORM as texture image format");
        }

        const VkSamplerCreateInfo sampler = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .magFilter = VK_FILTER_NEAREST,
            .minFilter = VK_FILTER_NEAREST,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_FALSE,
            .maxAnisotropy = 1,
            .compareOp = VK_COMPARE_OP_NEVER,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
            .unnormalizedCoordinates = VK_FALSE,
        };
        VkImageViewCreateInfo view = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = VK_NULL_HANDLE,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = texFormat,
            .components = {
                VK_COMPONENT_SWIZZLE_R,
                VK_COMPONENT_SWIZZLE_G,
                VK_COMPONENT_SWIZZLE_B,
                VK_COMPONENT_SWIZZLE_A,
            },
            .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 },
            .flags = 0,
        };

        /* create sampler */
        err = vkCreateSampler(demo->device, &sampler, nullptr,
                              &demo->textures[i].sampler);
        /// device 是创建采样器的逻辑设备。
        /// pCreateInfo 是一个指向VkSamplerCreateInfo结构的指针，该结构指定了采样器对象的状态。
        /// pAllocator 控制主机内存分配，如 内存分配章节所述。
        /// pSampler 是指向VkSampler句柄的指针，在该句柄中返回结果采样器对象。
        assert(!err);

        /* create image view */
        view.image = demo->textures[i].image;
        err = vkCreateImageView(demo->device, &view, nullptr,
                                &demo->textures[i].view);
        assert(!err);
    }
}

static void demoPrepareVertices(struct Demo* demo)
{
    // clang-format off
    const float vb[3][5] = {
        /*      position             texcoord */
        { 0.0f, -1.0f,  0.25f,     0.5f, 0.0f },
        {  1.0f, 1.0f,  0.25f,     1.0f, 1.0f },
        {  -1.0f,  1.0f,  0.25f,     0.0f, 1.0f },
    };
    // clang-format on
    const VkBufferCreateInfo bufInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .size = sizeof(vb),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .flags = 0,
    };
    VkMemoryAllocateInfo memAlloc = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = 0,
        .memoryTypeIndex = 0,
    };
    VkMemoryRequirements memReqs;
    VkResult U_ASSERT_ONLY err;
    bool U_ASSERT_ONLY pass;
    void* data;

    memset(&demo->vertices, 0, sizeof(demo->vertices));

    err = vkCreateBuffer(demo->device, &bufInfo, nullptr, &demo->vertices.buf);
    assert(!err);

    vkGetBufferMemoryRequirements(demo->device, demo->vertices.buf, &memReqs);
    assert(!err);

    memAlloc.allocationSize = memReqs.size;
    pass = memoryTypeFromProperties(demo, memReqs.memoryTypeBits,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                    &memAlloc.memoryTypeIndex);
    assert(pass);

    err = vkAllocateMemory(demo->device, &memAlloc, nullptr, &demo->vertices.mem);
    assert(!err);

    err = vkMapMemory(demo->device, demo->vertices.mem, 0,
                      memAlloc.allocationSize, 0, &data);
    assert(!err);

    memcpy(data, vb, sizeof(vb));

    vkUnmapMemory(demo->device, demo->vertices.mem);

    err = vkBindBufferMemory(demo->device, demo->vertices.buf,
                             demo->vertices.mem, 0);
    assert(!err);

    demo->vertices.vertexInputCreatInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    demo->vertices.vertexInputCreatInfo.pNext = nullptr;
    demo->vertices.vertexInputCreatInfo.vertexBindingDescriptionCount = 1;
    demo->vertices.vertexInputCreatInfo.pVertexBindingDescriptions = demo->vertices.vertexInputBindings;
    demo->vertices.vertexInputCreatInfo.vertexAttributeDescriptionCount = 2;
    demo->vertices.vertexInputCreatInfo.pVertexAttributeDescriptions = demo->vertices.vertexInputAttrs;

    demo->vertices.vertexInputBindings[0].binding = VERTEX_BUFFER_BIND_ID;
    demo->vertices.vertexInputBindings[0].stride = sizeof(vb[0]);
    demo->vertices.vertexInputBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    demo->vertices.vertexInputAttrs[0].binding = VERTEX_BUFFER_BIND_ID;
    demo->vertices.vertexInputAttrs[0].location = 0;
    demo->vertices.vertexInputAttrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    demo->vertices.vertexInputAttrs[0].offset = 0;

    demo->vertices.vertexInputAttrs[1].binding = VERTEX_BUFFER_BIND_ID;
    demo->vertices.vertexInputAttrs[1].location = 1;
    demo->vertices.vertexInputAttrs[1].format = VK_FORMAT_R32G32_SFLOAT;
    demo->vertices.vertexInputAttrs[1].offset = sizeof(float) * 3;
}

static void demoPrepareDescriptorLayout(struct Demo* demo)
{
    const VkDescriptorSetLayoutBinding layoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = DEMO_TEXTURE_COUNT,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    const VkDescriptorSetLayoutCreateInfo descriptorLayout = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .bindingCount = 1,
        .pBindings = &layoutBinding,
    };
    VkResult U_ASSERT_ONLY err;

    err = vkCreateDescriptorSetLayout(demo->device, &descriptorLayout, nullptr,
                                      &demo->descLayout);
    assert(!err);

    const VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 1,
        .pSetLayouts = &demo->descLayout,
    };

    err = vkCreatePipelineLayout(demo->device, &pPipelineLayoutCreateInfo, nullptr,
                                 &demo->pipelineLayout);
    assert(!err);
}

static void demoPrepareRenderPass(struct Demo* demo)
{
    const VkAttachmentDescription attachments[2] = {
        {
            .format = demo->format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        },
        {
            .format = demo->depth.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        },
    };
    const VkAttachmentReference colorReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentReference depthReference = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    const VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .flags = 0,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = &depthReference,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };
    const VkRenderPassCreateInfo rpInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .attachmentCount = 2,
        .pAttachments = attachments,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 0,
        .pDependencies = nullptr,
    };
    VkResult U_ASSERT_ONLY err;

    err = vkCreateRenderPass(demo->device, &rpInfo, nullptr, &demo->renderPass);
    assert(!err);
}

static VkShaderModule
    demoPrepareShaderModule(struct Demo* demo, const void* code, size_t size)
{
    VkShaderModuleCreateInfo moduleCreateInfo;
    VkShaderModule module;
    VkResult U_ASSERT_ONLY err;

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = nullptr;

    moduleCreateInfo.codeSize = size;
    moduleCreateInfo.pCode = static_cast<const uint32_t*>(code);
    moduleCreateInfo.flags = 0;
    err = vkCreateShaderModule(demo->device, &moduleCreateInfo, nullptr, &module);
    assert(!err);

    return module;
}

static VkShaderModule demoPrepareVertShader(struct Demo* demo)
{
    size_t size = sizeof(vertShaderCode);

    demo->vertShaderModule =
        demoPrepareShaderModule(demo, vertShaderCode, size);

    return demo->vertShaderModule;
}

static VkShaderModule demoPrepareFragShader(struct Demo* demo)
{
    size_t size = sizeof(fragShaderCode);

    demo->fragShaderModule =
        demoPrepareShaderModule(demo, fragShaderCode, size);

    return demo->fragShaderModule;
}

static void demoPreparePipeline(struct Demo* demo)
{
    VkGraphicsPipelineCreateInfo pipelineInfo;
    VkPipelineCacheCreateInfo pipelineCache;

    VkPipelineVertexInputStateCreateInfo vi;
    VkPipelineInputAssemblyStateCreateInfo ia;
    VkPipelineRasterizationStateCreateInfo rs;
    VkPipelineColorBlendStateCreateInfo cb;
    VkPipelineDepthStencilStateCreateInfo ds;
    VkPipelineViewportStateCreateInfo vp;
    VkPipelineMultisampleStateCreateInfo ms;
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_STENCIL_REFERENCE - VK_DYNAMIC_STATE_VIEWPORT + 1];
    VkPipelineDynamicStateCreateInfo dynamicState;

    VkResult U_ASSERT_ONLY err;

    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    memset(&dynamicState, 0, sizeof dynamicState);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables;

    memset(&pipelineInfo, 0, sizeof(pipelineInfo));
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = demo->pipelineLayout;

    vi = demo->vertices.vertexInputCreatInfo;

    memset(&ia, 0, sizeof(ia));
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    memset(&rs, 0, sizeof(rs));
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rs.depthClampEnable = VK_FALSE;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.lineWidth = 1.0f;

    memset(&cb, 0, sizeof(cb));
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    VkPipelineColorBlendAttachmentState att_state[1];
    memset(att_state, 0, sizeof(att_state));
    att_state[0].colorWriteMask = 0xf;
    att_state[0].blendEnable = VK_FALSE;
    cb.attachmentCount = 1;
    cb.pAttachments = att_state;

    memset(&vp, 0, sizeof(vp));
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.viewportCount = 1;
    dynamicStateEnables[dynamicState.dynamicStateCount++] =
        VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = 1;
    dynamicStateEnables[dynamicState.dynamicStateCount++] =
        VK_DYNAMIC_STATE_SCISSOR;

    memset(&ds, 0, sizeof(ds));
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.depthTestEnable = VK_TRUE;
    ds.depthWriteEnable = VK_TRUE;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.stencilTestEnable = VK_FALSE;
    ds.front = ds.back;

    memset(&ms, 0, sizeof(ms));
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pSampleMask = nullptr;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Two stages: vs and fs
    pipelineInfo.stageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    memset(&shaderStages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = demoPrepareVertShader(demo);
    shaderStages[0].pName = "main";

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = demoPrepareFragShader(demo);
    shaderStages[1].pName = "main";

    pipelineInfo.pVertexInputState = &vi;
    pipelineInfo.pInputAssemblyState = &ia;
    pipelineInfo.pRasterizationState = &rs;
    pipelineInfo.pColorBlendState = &cb;
    pipelineInfo.pMultisampleState = &ms;
    pipelineInfo.pViewportState = &vp;
    pipelineInfo.pDepthStencilState = &ds;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.renderPass = demo->renderPass;
    pipelineInfo.pDynamicState = &dynamicState;

    memset(&pipelineCache, 0, sizeof(pipelineCache));
    pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    err = vkCreatePipelineCache(demo->device, &pipelineCache, nullptr,
                                &demo->pipelineCache);
    assert(!err);
    err = vkCreateGraphicsPipelines(demo->device, demo->pipelineCache, 1,
                                    &pipelineInfo, nullptr, &demo->pipeline);
    assert(!err);

    vkDestroyPipelineCache(demo->device, demo->pipelineCache, nullptr);

    vkDestroyShaderModule(demo->device, demo->fragShaderModule, nullptr);
    vkDestroyShaderModule(demo->device, demo->vertShaderModule, nullptr);
}

static void demoPrepareDescriptorPool(struct Demo* demo)
{
    const VkDescriptorPoolSize typeCount = {
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = DEMO_TEXTURE_COUNT,
    };
    const VkDescriptorPoolCreateInfo descriptorPool = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .maxSets = 1,
        .poolSizeCount = 1,
        .pPoolSizes = &typeCount,
    };
    VkResult U_ASSERT_ONLY err;

    err = vkCreateDescriptorPool(demo->device, &descriptorPool, nullptr,
                                 &demo->descPool);
    assert(!err);
}

static void demoPrepareDescriptorSet(struct Demo* demo)
{
    VkDescriptorImageInfo texDescImageInfo[DEMO_TEXTURE_COUNT];
    VkWriteDescriptorSet write;
    VkResult U_ASSERT_ONLY err;

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = demo->descPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &demo->descLayout
    };
    err = vkAllocateDescriptorSets(demo->device, &allocInfo, &demo->descSet);
    assert(!err);

    memset(&texDescImageInfo, 0, sizeof(texDescImageInfo));
    for (uint32_t i = 0; i < DEMO_TEXTURE_COUNT; ++i)
    {
        texDescImageInfo[i].sampler = demo->textures[i].sampler;
        texDescImageInfo[i].imageView = demo->textures[i].view;
        texDescImageInfo[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    }

    memset(&write, 0, sizeof(write));
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = demo->descSet;
    write.descriptorCount = DEMO_TEXTURE_COUNT;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.pImageInfo = texDescImageInfo;

    vkUpdateDescriptorSets(demo->device, 1, &write, 0, nullptr);
}

static void demoPrepareFramebuffers(struct Demo* demo)
{
    VkImageView attachments[2];
    attachments[1] = demo->depth.view;

    const VkFramebufferCreateInfo fbInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .renderPass = demo->renderPass,
        .attachmentCount = 2,
        .pAttachments = attachments,
        .width = static_cast<uint32_t>(demo->width),
        .height = static_cast<uint32_t>(demo->height),
        .layers = 1,
    };
    VkResult U_ASSERT_ONLY err;

    demo->frameBuffers = (VkFramebuffer*)malloc(demo->swapChainImageCount *
                                                sizeof(VkFramebuffer));
    assert(demo->frameBuffers);

    for (uint32_t i = 0; i < demo->swapChainImageCount; ++i)
    {
        attachments[0] = demo->swapChainBuffers[i].view;
        err = vkCreateFramebuffer(demo->device, &fbInfo, nullptr,
                                  &demo->frameBuffers[i]);
        assert(!err);
    }
}

static void demoPrepare(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;

    const VkCommandPoolCreateInfo cmdPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .queueFamilyIndex = demo->graphicsQueueNodeIndex,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    };
    err = vkCreateCommandPool(demo->device, &cmdPoolInfo, nullptr,
                              &demo->cmdPool);
    assert(!err);

    const VkCommandBufferAllocateInfo cmd = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = demo->cmdPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    err = vkAllocateCommandBuffers(demo->device, &cmd, &demo->drawCmd);
    assert(!err);

    demoPrepareBuffers(demo);
    demo_prepare_depth(demo);
    demoPrepareTextures(demo);
    demoPrepareVertices(demo);
    demoPrepareDescriptorLayout(demo);
    demoPrepareRenderPass(demo);
    demoPreparePipeline(demo);

    demoPrepareDescriptorPool(demo);
    demoPrepareDescriptorSet(demo);

    demoPrepareFramebuffers(demo);
}

static void demoErrorCallback(int error, const char* description)
{
    printf("GLFW error: %s\n", description);
    fflush(stdout);
}

static void demoKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void demoRefreshCallback(GLFWwindow* window)
{
    //    struct Demo* Demo = glfwGetWindowUserPointer(window);
    //    demoDraw(Demo);
}

static void demoResizeCallback(GLFWwindow* window, int width, int height)
{
    //    struct Demo* Demo = glfwGetWindowUserPointer(window);
    //    Demo->width = width;
    //    Demo->height = height;
    //    demoResize(Demo);
}

static void demoRun(struct Demo* demo)
{
    while (!glfwWindowShouldClose(demo->window))
    {
        glfwPollEvents();

        demoDraw(demo);

        if (demo->depthStencil > 0.99f)
            demo->depthIncrement = -0.001f;
        if (demo->depthStencil < 0.8f)
            demo->depthIncrement = 0.001f;

        demo->depthStencil += demo->depthIncrement;

        // Wait for work to finish before updating MVP.
        vkDeviceWaitIdle(demo->device);
        demo->curFrame++;
        if (demo->frameCount != INT32_MAX && demo->curFrame == demo->frameCount)
            glfwSetWindowShouldClose(demo->window, GLFW_TRUE);
    }
}

static void demoCreateWindow(struct Demo* demo)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    demo->window = glfwCreateWindow(demo->width,
                                    demo->height,
                                    APP_LONG_NAME,
                                    nullptr,
                                    nullptr);
    if (!demo->window)
    {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }

    glfwSetWindowUserPointer(demo->window, demo);
    glfwSetWindowRefreshCallback(demo->window, demoRefreshCallback);
    glfwSetFramebufferSizeCallback(demo->window, demoResizeCallback);
    glfwSetKeyCallback(demo->window, demoKeyCallback);
}

/*
 * Return 1 (true) if all layer names specified in checkNames
 * can be found in given layer properties.
 */
static VkBool32 demoCheckLayers(uint32_t checkCount, const char** checkNames,
                                uint32_t layerCount,
                                VkLayerProperties* layers)
{
    for (uint32_t i = 0; i < checkCount; ++i)
    {
        VkBool32 found = 0;
        for (uint32_t j = 0; j < layerCount; ++j)
        {
            if (!strcmp(checkNames[i], layers[j].layerName))
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "Cannot find layer: %s\n", checkNames[i]);
            return 0;
        }
    }
    return 1;
}

static void demoInitVk(struct Demo* demo)
{
    VkResult err;
    uint32_t requiredExtensionCount = 0;
    uint32_t instanceExtensionCount = 0;
    uint32_t instanceLayerCount = 0;
    uint32_t validationLayerCount = 0;
    const char** requiredExtensions = nullptr;
    const char** instanceValidationLayers = nullptr;
    demo->enabledExtensionCount = 0;
    demo->enabledLayerCount = 0;

    constexpr const char*  instanceValidationLayersAlt1[] = {
        "VK_LAYER_LUNARG_standard_validation"
    };

    constexpr const char* instanceValidationLayersAlt2[] = {
        "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker", "VK_LAYER_LUNARG_image",
        "VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects"
    };

    /* Look for validation layers */
    VkBool32 validationFound = 0;
    if (demo->validate)
    {
        err = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        assert(!err);

        instanceValidationLayers = (const char**)instanceValidationLayersAlt1;
        if (instanceLayerCount > 0)
        {
            auto* instanceLayers =
                static_cast<VkLayerProperties*>(malloc(sizeof(VkLayerProperties) * instanceLayerCount));
            err = vkEnumerateInstanceLayerProperties(&instanceLayerCount,
                                                     instanceLayers);
            assert(!err);

            validationFound = demoCheckLayers(
                ARRAY_SIZE(instanceValidationLayersAlt1),
                instanceValidationLayers, instanceLayerCount,
                instanceLayers);
            if (validationFound)
            {
                demo->enabledLayerCount = ARRAY_SIZE(instanceValidationLayersAlt1);
                demo->enabledLayers[0] = "VK_LAYER_LUNARG_standard_validation";
                validationLayerCount = 1;
            }
            else
            {
                // use alternative set of validation layers
                instanceValidationLayers =
                    (const char**)instanceValidationLayersAlt2;
                demo->enabledLayerCount = ARRAY_SIZE(instanceValidationLayersAlt2);
                validationFound = demoCheckLayers(
                    ARRAY_SIZE(instanceValidationLayersAlt2),
                    instanceValidationLayers, instanceLayerCount,
                    instanceLayers);
                validationLayerCount =
                    ARRAY_SIZE(instanceValidationLayersAlt2);
                for (uint32_t i = 0; i < validationLayerCount; ++i)
                {
                    demo->enabledLayers[i] = instanceValidationLayers[i];
                }
            }
            free(instanceLayers);
        }

        if (!validationFound)
        {
            ERR_EXIT("vkEnumerateInstanceLayerProperties failed to find "
                     "required validation layer.\n\n"
                     "Please look at the Getting Started guide for additional "
                     "information.\n",
                     "vkCreateInstance Failure");
        }
    }

    /* Look for instance extensions */
    requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
    if (!requiredExtensions)
    {
        ERR_EXIT("glfwGetRequiredInstanceExtensions failed to find the "
                 "platform surface extensions.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
    }

    for (uint32_t i = 0; i < requiredExtensionCount; ++i)
    {
        demo->extensionNames[demo->enabledExtensionCount++] = requiredExtensions[i];
        assert(demo->enabledExtensionCount < 64);
    }

    err = vkEnumerateInstanceExtensionProperties(
        nullptr, &instanceExtensionCount, nullptr);
    assert(!err);

    if (instanceExtensionCount > 0)
    {
        auto* instanceExtensions =
            static_cast<VkExtensionProperties*>(malloc(sizeof(VkExtensionProperties) * instanceExtensionCount));
        err = vkEnumerateInstanceExtensionProperties(
            nullptr, &instanceExtensionCount, instanceExtensions);
        assert(!err);
        for (uint32_t i = 0; i < instanceExtensionCount; ++i)
        {
            if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                        instanceExtensions[i].extensionName))
            {
                if (demo->validate)
                {
                    demo->extensionNames[demo->enabledExtensionCount++] =
                        VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
                }
            }
            assert(demo->enabledExtensionCount < 64);
        }

        free(instanceExtensions);
    }

    const VkApplicationInfo app = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = APP_SHORT_NAME,
        .applicationVersion = 0,
        .pEngineName = APP_SHORT_NAME,
        .engineVersion = 0,
        .apiVersion = VK_API_VERSION_1_0,
    };
    VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &app,
        .enabledLayerCount = demo->enabledLayerCount,
        .ppEnabledLayerNames = (const char* const*)instanceValidationLayers,
        .enabledExtensionCount = demo->enabledExtensionCount,
        .ppEnabledExtensionNames = (const char* const*)demo->extensionNames,
    };

    uint32_t gpuCount;
    vkCreateInstance(&instanceInfo, nullptr, &demo->instance);
    /// pCreateInfo是一个指向控制实例创建的VkInstanceCreateInfo结构的指针。
    /// pAllocator控制主机内存分配
    /// pInstance指向返回结果实例的VkInstance句柄。

    /* Make initial call to query gpuCount, then second call for gpu info*/
    err = vkEnumeratePhysicalDevices(demo->instance, &gpuCount, nullptr);
    assert(!err && gpuCount > 0);

    if (gpuCount > 0)
    {
        auto* physicalDevices =
            static_cast<VkPhysicalDevice*>(malloc(sizeof(VkPhysicalDevice) * gpuCount));
        err = vkEnumeratePhysicalDevices(demo->instance, &gpuCount,
                                         physicalDevices);
        assert(!err);
        /* For tri Demo we just grab the first physical device */
        demo->gpu = physicalDevices[0];
        free(physicalDevices);
    }
    else
    {
        ERR_EXIT("vkEnumeratePhysicalDevices reported zero accessible devices."
                 "\n\nDo you have a compatible Vulkan installable client"
                 " driver (ICD) installed?\nPlease look at the Getting Started"
                 " guide for additional information.\n",
                 "vkEnumeratePhysicalDevices Failure");
    }

    //    gladLoadVulkanUserPtr(Demo->gpu, glad_vulkan_callback, Demo->instance);

    /* Look for device extensions */
    uint32_t deviceExtensionCount = 0;
    VkBool32 swapchainExtFound = 0;
    demo->enabledExtensionCount = 0;

    err = vkEnumerateDeviceExtensionProperties(demo->gpu, nullptr,
                                               &deviceExtensionCount, nullptr);
    assert(!err);

    if (deviceExtensionCount > 0)
    {
        auto* deviceExtensions =
            static_cast<VkExtensionProperties*>(malloc(sizeof(VkExtensionProperties) * deviceExtensionCount));
        err = vkEnumerateDeviceExtensionProperties(
            demo->gpu, nullptr, &deviceExtensionCount, deviceExtensions);
        assert(!err);

        for (uint32_t i = 0; i < deviceExtensionCount; ++i)
        {
            if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                        deviceExtensions[i].extensionName))
            {
                swapchainExtFound = 1;
                demo->extensionNames[demo->enabledExtensionCount++] =
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            }
            assert(demo->enabledExtensionCount < 64);
        }

        free(deviceExtensions);
    }

    if (!swapchainExtFound)
    {
        ERR_EXIT("vkEnumerateDeviceExtensionProperties failed to find "
                 "the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
    }

    if (demo->validate)
    {
        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.flags =
            VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        dbgCreateInfo.pfnCallback = demo->use_break ? BreakCallback : dbgFunc;
        dbgCreateInfo.pUserData = demo;
        dbgCreateInfo.pNext = nullptr;
//        err = vkCreateDebugReportCallbackEXT(demo->instance, &dbgCreateInfo, nullptr,
//                                             &demo->msgCallback);
//        switch (err)
//        {
//        case VK_SUCCESS:
//            break;
//        case VK_ERROR_OUT_OF_HOST_MEMORY:
//            ERR_EXIT("CreateDebugReportCallback: out of host memory\n",
//                     "CreateDebugReportCallback Failure");
//            break;
//        default:
//            ERR_EXIT("CreateDebugReportCallback: unknown failure\n",
//                     "CreateDebugReportCallback Failure");
//            break;
//        }
    }

    vkGetPhysicalDeviceProperties(demo->gpu, &demo->gpuProperties);

    // Query with NULL data to get count
    vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queueCount,
                                             nullptr);

    demo->queueProperties = (VkQueueFamilyProperties*)malloc(
        demo->queueCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queueCount,
                                             demo->queueProperties);
    assert(demo->queueCount >= 1);

    vkGetPhysicalDeviceFeatures(demo->gpu, &demo->gpuFeatures);

    // Graphics queue and MemMgr queue can be separate.
    // TODO: Add support for separate queues, including synchronization,
    //       and appropriate tracking for QueueSubmit
}

static void demoInitDevice(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;

    float queuePriorities[1] = { 0.0f };
    const VkDeviceQueueCreateInfo queue = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .queueFamilyIndex = demo->graphicsQueueNodeIndex,
        .queueCount = 1,
        .pQueuePriorities = queuePriorities
    };

    VkPhysicalDeviceFeatures features;
    memset(&features, 0, sizeof(features));
    if (demo->gpuFeatures.shaderClipDistance)
    {
        features.shaderClipDistance = VK_TRUE;
    }

    VkDeviceCreateInfo device = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = demo->enabledExtensionCount,
        .ppEnabledExtensionNames = (const char* const*)demo->extensionNames,
        .pEnabledFeatures = &features,
    };

    err = vkCreateDevice(demo->gpu, &device, nullptr, &demo->device);
    assert(!err);
}

static void demoInitVkSwapchain(struct Demo* demo)
{
    VkResult U_ASSERT_ONLY err;

    // Create a WSI surface for the window:
    glfwCreateWindowSurface(demo->instance, demo->window, nullptr, &demo->surface);

    // Iterate over each queue to learn whether it supports presenting:
    auto* supportsPresent = (VkBool32*)malloc(demo->queueCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < demo->queueCount; ++i)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(demo->gpu, i, demo->surface, &supportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < demo->queueCount; ++i)
    {
        if ((demo->queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (graphicsQueueNodeIndex == UINT32_MAX)
            {
                graphicsQueueNodeIndex = i;
            }

            if (supportsPresent[i] == VK_TRUE)
            {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }
    }
    if (presentQueueNodeIndex == UINT32_MAX)
    {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (uint32_t i = 0; i < demo->queueCount; ++i)
        {
            if (supportsPresent[i] == VK_TRUE)
            {
                presentQueueNodeIndex = i;
                break;
            }
        }
    }
    free(supportsPresent);

    // Generate error if could not find both a graphics and a present queue
    if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
    {
        ERR_EXIT("Could not find a graphics and a present queue\n",
                 "Swapchain Initialization Failure");
    }

    // TODO: Add support for separate queues, including presentation,
    //       synchronization, and appropriate tracking for QueueSubmit.
    // NOTE: While it is possible for an application to use a separate graphics
    //       and a present queues, this Demo program assumes it is only using
    //       one:
    if (graphicsQueueNodeIndex != presentQueueNodeIndex)
    {
        ERR_EXIT("Could not find a common graphics and a present queue\n",
                 "Swapchain Initialization Failure");
    }

    demo->graphicsQueueNodeIndex = graphicsQueueNodeIndex;

    demoInitDevice(demo);

    vkGetDeviceQueue(demo->device, demo->graphicsQueueNodeIndex, 0,
                     &demo->queue);

    // Get the list of VkFormat's that are supported:
    uint32_t formatCount;
    err = vkGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface, &formatCount, nullptr);
    /// physicalDevice是将与要创建的交换链关联的物理设备，如 vkCreateSwapchainKHR 所述。
    /// pSurfaceInfo是一个指向 VkPhysicalDeviceSurfaceInfo2KHR结构描述表面和将由被消耗其它固定参数 vkCreateSwapchainKHR。
    /// pSurfaceFormatCount 是一个指向与可用或查询的格式元组数量相关的整数的指针，如下所述。
    /// pSurfaceFormats是NULL或指向VkSurfaceFormat2KHR结构数组的指针 。

    assert(!err);
    auto* surfFormats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    /// format是与指定表面兼容的VkFormat。
    /// colorSpace是与表面兼容的演示文稿VkColorSpaceKHR。
    err = vkGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface, &formatCount, surfFormats);
    assert(!err);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        demo->format = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else
    {
        assert(formatCount >= 1);
        demo->format = surfFormats[0].format;
    }
    demo->colorSpace = surfFormats[0].colorSpace;

    demo->curFrame = 0;

    // Get Memory information and properties
    vkGetPhysicalDeviceMemoryProperties(demo->gpu, &demo->memoryProperties);
}

static void demoInitConnection(struct Demo* demo)
{
    glfwSetErrorCallback(demoErrorCallback);

    if (!glfwInit())
    {
        printf("Cannot initialize GLFW.\nExiting ...\n");
        fflush(stdout);
        exit(1);
    }

    if (!glfwVulkanSupported())
    {
        printf("GLFW failed to find the Vulkan loader.\nExiting ...\n");
        fflush(stdout);
        exit(1);
    }

    //    gladLoadVulkanUserPtr(nullptr, glad_vulkan_callback, NULL);
}

static void demoInit(struct Demo* demo, const int argc, const char** argv)
{
    memset(demo, 0, sizeof(*demo));
    demo->frameCount = INT32_MAX;

    for (uint32_t i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--use_staging") == 0)
        {
            demo->useStagingBuffer = true;
            continue;
        }
        if (strcmp(argv[i], "--break") == 0)
        {
            demo->use_break = true;
            continue;
        }
        if (strcmp(argv[i], "--validate") == 0)
        {
            demo->validate = true;
            continue;
        }
        if (strcmp(argv[i], "--c") == 0 && demo->frameCount == INT32_MAX &&
            i < argc - 1 && sscanf(argv[i + 1], "%d", &demo->frameCount) == 1 &&
            demo->frameCount >= 0)
        {
            i++;
            continue;
        }

        fprintf(stderr, "Usage:\n  %s [--use_staging] [--validate] [--break] "
                        "[--c <framecount>]\n",
                APP_SHORT_NAME);
        fflush(stderr);
        exit(1);
    }

    demoInitConnection(demo);
    demoInitVk(demo);

    demo->width = 300;
    demo->height = 300;
    demo->depthStencil = 1.0;
    demo->depthIncrement = -0.01f;
}

static void demoCleanup(struct Demo* demo)
{
    for (uint32_t i = 0; i < demo->swapChainImageCount; ++i)
    {
        vkDestroyFramebuffer(demo->device, demo->frameBuffers[i], nullptr);
    }
    free(demo->frameBuffers);
    vkDestroyDescriptorPool(demo->device, demo->descPool, nullptr);

    if (demo->setupCmd)
    {
        vkFreeCommandBuffers(demo->device, demo->cmdPool, 1, &demo->setupCmd);
    }
    vkFreeCommandBuffers(demo->device, demo->cmdPool, 1, &demo->drawCmd);
    vkDestroyCommandPool(demo->device, demo->cmdPool, nullptr);

    vkDestroyPipeline(demo->device, demo->pipeline, nullptr);
    vkDestroyRenderPass(demo->device, demo->renderPass, nullptr);
    vkDestroyPipelineLayout(demo->device, demo->pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(demo->device, demo->descLayout, nullptr);

    vkDestroyBuffer(demo->device, demo->vertices.buf, nullptr);
    vkFreeMemory(demo->device, demo->vertices.mem, nullptr);

    for (auto & texture : demo->textures)
    {
        vkDestroyImageView(demo->device, texture.view, nullptr);
        vkDestroyImage(demo->device, texture.image, nullptr);
        vkFreeMemory(demo->device, texture.mem, nullptr);
        vkDestroySampler(demo->device, texture.sampler, nullptr);
    }

    for (uint32_t i = 0; i < demo->swapChainImageCount; ++i)
    {
        vkDestroyImageView(demo->device, demo->swapChainBuffers[i].view, nullptr);
    }

    vkDestroyImageView(demo->device, demo->depth.view, nullptr);
    vkDestroyImage(demo->device, demo->depth.image, nullptr);
    vkFreeMemory(demo->device, demo->depth.mem, nullptr);

    vkDestroySwapchainKHR(demo->device, demo->swapChain, nullptr);
    free(demo->swapChainBuffers);

    vkDestroyDevice(demo->device, nullptr);
    if (demo->validate)
    {
//        vkDestroyDebugReportCallbackEXT(demo->instance, demo->msgCallback, nullptr);
    }
    vkDestroySurfaceKHR(demo->instance, demo->surface, nullptr);
    /// 创建后，VkSurfaceKHR对象可以在此扩展和其他扩展中使用，尤其是在VK_KHR_swapchain扩展中。
    /// 要销毁VkSurfaceKHR对象:调用vkDestroySurfaceKHR
    /// instance 是用于创建表面的实例。
    /// surface 是要破坏的表面。
    /// pAllocator是当没有更多特定分配器可用时，用于为表面对象分配的主机内存的分配器
    vkDestroyInstance(demo->instance, nullptr);

    free(demo->queueProperties);

    glfwDestroyWindow(demo->window);
    glfwTerminate();
}

static void demoResize(struct Demo* demo)
{
    // In order to properly resize the window, we must re-create the swapChain
    // AND redo the command swapChainBuffers, etc.
    //
    // First, perform part of the demoCleanup() function:

    for (uint32_t i = 0; i < demo->swapChainImageCount; ++i)
    {
        vkDestroyFramebuffer(demo->device, demo->frameBuffers[i], nullptr);
    }
    free(demo->frameBuffers);
    vkDestroyDescriptorPool(demo->device, demo->descPool, nullptr);

    if (demo->setupCmd)
    {
        vkFreeCommandBuffers(demo->device, demo->cmdPool, 1, &demo->setupCmd);
    }
    vkFreeCommandBuffers(demo->device, demo->cmdPool, 1, &demo->drawCmd);
    vkDestroyCommandPool(demo->device, demo->cmdPool, nullptr);

    vkDestroyPipeline(demo->device, demo->pipeline, nullptr);
    vkDestroyRenderPass(demo->device, demo->renderPass, nullptr);
    vkDestroyPipelineLayout(demo->device, demo->pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(demo->device, demo->descLayout, nullptr);

    vkDestroyBuffer(demo->device, demo->vertices.buf, nullptr);
    vkFreeMemory(demo->device, demo->vertices.mem, nullptr);

    for (auto & texture : demo->textures)
    {
        vkDestroyImageView(demo->device, texture.view, nullptr);
        vkDestroyImage(demo->device, texture.image, nullptr);
        vkFreeMemory(demo->device, texture.mem, nullptr);
        vkDestroySampler(demo->device, texture.sampler, nullptr);
    }

    for (uint32_t i = 0; i < demo->swapChainImageCount; ++i)
    {
        vkDestroyImageView(demo->device, demo->swapChainBuffers[i].view, nullptr);
    }

    vkDestroyImageView(demo->device, demo->depth.view, nullptr);
    vkDestroyImage(demo->device, demo->depth.image, nullptr);
    vkFreeMemory(demo->device, demo->depth.mem, nullptr);

    free(demo->swapChainBuffers);

    // Second, re-perform the demoPrepare() function, which will re-create the
    // swapChain:
    demoPrepare(demo);
}

//#include "Tutorial01.h"
int main(const int argc, const char* argv[])
{
    struct Demo demo{};
    demoInit(&demo, argc, argv);
    demoCreateWindow(&demo);
    demoInitVkSwapchain(&demo);
    demoPrepare(&demo);
    demoRun(&demo);
    demoCleanup(&demo);
    return 0;
}
