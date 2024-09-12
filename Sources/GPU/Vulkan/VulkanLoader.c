// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/Vulkan/VulkanLoader.h>

#include <Core/LibLoader.h>
#include <Core/Logs.h>

static KbhLibModule __kbh_vulkan_lib_module = KBH_NULL_LIB_MODULE;

static void kbhVulkanLoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void kvhVulkanLoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void kbhVulkanLoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*));

static inline PFN_vkVoidFunction vkGetInstanceProcAddrStub(void* context, const char* name)
{
	return vkGetInstanceProcAddr((VkInstance)context, name);
}

VkResult kbhVulkanLoaderInit()
{
	#if defined(KANEL_CLI_PLAT_WINDOWS)
		__kbh_vulkan_lib_module = kbhLoadLibrary("vulkan-1.dll");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#elif defined(KANEL_CLI_PLAT_MACOS)
		__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.dylib");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.1.dylib");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libMoltenVK.dylib");

		// Add support for using Vulkan and MoltenVK in a Framework. App store rules for iOS
		// strictly enforce no .dylib's. If they aren't found it just falls through
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("vulkan.framework/vulkan");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("MoltenVK.framework/MoltenVK");

		// modern versions of macOS don't search /usr/local/lib automatically contrary to what man dlopen says
		// Vulkan SDK uses this as the system-wide installation location, so we're going to fallback to this if all else fails
		if(!__kbh_vulkan_lib_module && getenv("DYLD_FALLBACK_LIBRARY_PATH") == NULL)
			__kbh_vulkan_lib_module = kbhLoadLibrary("/usr/local/lib/libvulkan.dylib");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#else
		__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.so.1");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.so");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#endif
	vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)kbhLoadSymbolFromLibModule(__kbh_vulkan_lib_module, "vkGetInstanceProcAddr");
	kbhDebugLog("Vulkan RHI : vulkan library loaded");
	kbhVulkanLoadGlobalFunctions(KANEL_CLI_NULLPTR, vkGetInstanceProcAddrStub);

	return VK_SUCCESS;
}

void kbhLoadInstance(VkInstance instance)
{
	kbhVulkanLoadInstanceFunctions(instance, vkGetInstanceProcAddrStub);
	kbhVulkanLoadDeviceFunctions(instance, vkGetInstanceProcAddrStub);
}

void kbhVulkanLoaderUninit()
{
	kbhUnloadLibrary(__kbh_vulkan_lib_module);
	__kbh_vulkan_lib_module = KBH_NULL_LIB_MODULE;
	kbhDebugLog("Vulkan RHI : vulkan library unloaded");
}

static void kbhVulkanLoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#if defined(VK_VERSION_1_0)
		vkCreateInstance = (PFN_vkCreateInstance)load(context, "vkCreateInstance");
		vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)load(context, "vkEnumerateInstanceExtensionProperties");
		vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)load(context, "vkEnumerateInstanceLayerProperties");
	#endif /* defined(VK_VERSION_1_0) */
	kbhDebugLog("Vulkan RHI : vulkan global functions loaded");
}

static void kvhVulkanLoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#if defined(VK_VERSION_1_0)
		vkCreateDevice = (PFN_vkCreateDevice)load(context, "vkCreateDevice");
		vkDestroyInstance = (PFN_vkDestroyInstance)load(context, "vkDestroyInstance");
		vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)load(context, "vkEnumerateDeviceExtensionProperties");
		vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)load(context, "vkEnumerateDeviceLayerProperties");
		vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)load(context, "vkEnumeratePhysicalDevices");
		vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)load(context, "vkGetDeviceProcAddr");
		vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)load(context, "vkGetPhysicalDeviceFeatures");
		vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)load(context, "vkGetPhysicalDeviceFormatProperties");
		vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)load(context, "vkGetPhysicalDeviceImageFormatProperties");
		vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)load(context, "vkGetPhysicalDeviceMemoryProperties");
		vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)load(context, "vkGetPhysicalDeviceProperties");
		vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)load(context, "vkGetPhysicalDeviceQueueFamilyProperties");
		vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties");
	#endif /* defined(VK_VERSION_1_0) */
	kbhDebugLog("Vulkan RHI : vulkan instance function loaded");
}

static void kbhVulkanLoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#if defined(VK_VERSION_1_0)
		vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)load(context, "vkAllocateCommandBuffers");
		vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)load(context, "vkAllocateDescriptorSets");
		vkAllocateMemory = (PFN_vkAllocateMemory)load(context, "vkAllocateMemory");
		vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)load(context, "vkBeginCommandBuffer");
		vkBindBufferMemory = (PFN_vkBindBufferMemory)load(context, "vkBindBufferMemory");
		vkBindImageMemory = (PFN_vkBindImageMemory)load(context, "vkBindImageMemory");
		vkCmdBeginQuery = (PFN_vkCmdBeginQuery)load(context, "vkCmdBeginQuery");
		vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)load(context, "vkCmdBeginRenderPass");
		vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)load(context, "vkCmdBindDescriptorSets");
		vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)load(context, "vkCmdBindIndexBuffer");
		vkCmdBindPipeline = (PFN_vkCmdBindPipeline)load(context, "vkCmdBindPipeline");
		vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)load(context, "vkCmdBindVertexBuffers");
		vkCmdBlitImage = (PFN_vkCmdBlitImage)load(context, "vkCmdBlitImage");
		vkCmdClearAttachments = (PFN_vkCmdClearAttachments)load(context, "vkCmdClearAttachments");
		vkCmdClearColorImage = (PFN_vkCmdClearColorImage)load(context, "vkCmdClearColorImage");
		vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)load(context, "vkCmdClearDepthStencilImage");
		vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)load(context, "vkCmdCopyBuffer");
		vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)load(context, "vkCmdCopyBufferToImage");
		vkCmdCopyImage = (PFN_vkCmdCopyImage)load(context, "vkCmdCopyImage");
		vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)load(context, "vkCmdCopyImageToBuffer");
		vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)load(context, "vkCmdCopyQueryPoolResults");
		vkCmdDispatch = (PFN_vkCmdDispatch)load(context, "vkCmdDispatch");
		vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)load(context, "vkCmdDispatchIndirect");
		vkCmdDraw = (PFN_vkCmdDraw)load(context, "vkCmdDraw");
		vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)load(context, "vkCmdDrawIndexed");
		vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)load(context, "vkCmdDrawIndexedIndirect");
		vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)load(context, "vkCmdDrawIndirect");
		vkCmdEndQuery = (PFN_vkCmdEndQuery)load(context, "vkCmdEndQuery");
		vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)load(context, "vkCmdEndRenderPass");
		vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)load(context, "vkCmdExecuteCommands");
		vkCmdFillBuffer = (PFN_vkCmdFillBuffer)load(context, "vkCmdFillBuffer");
		vkCmdNextSubpass = (PFN_vkCmdNextSubpass)load(context, "vkCmdNextSubpass");
		vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)load(context, "vkCmdPipelineBarrier");
		vkCmdPushConstants = (PFN_vkCmdPushConstants)load(context, "vkCmdPushConstants");
		vkCmdResetEvent = (PFN_vkCmdResetEvent)load(context, "vkCmdResetEvent");
		vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)load(context, "vkCmdResetQueryPool");
		vkCmdResolveImage = (PFN_vkCmdResolveImage)load(context, "vkCmdResolveImage");
		vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)load(context, "vkCmdSetBlendConstants");
		vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)load(context, "vkCmdSetDepthBias");
		vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)load(context, "vkCmdSetDepthBounds");
		vkCmdSetEvent = (PFN_vkCmdSetEvent)load(context, "vkCmdSetEvent");
		vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)load(context, "vkCmdSetLineWidth");
		vkCmdSetScissor = (PFN_vkCmdSetScissor)load(context, "vkCmdSetScissor");
		vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)load(context, "vkCmdSetStencilCompareMask");
		vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)load(context, "vkCmdSetStencilReference");
		vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)load(context, "vkCmdSetStencilWriteMask");
		vkCmdSetViewport = (PFN_vkCmdSetViewport)load(context, "vkCmdSetViewport");
		vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)load(context, "vkCmdUpdateBuffer");
		vkCmdWaitEvents = (PFN_vkCmdWaitEvents)load(context, "vkCmdWaitEvents");
		vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)load(context, "vkCmdWriteTimestamp");
		vkCreateBuffer = (PFN_vkCreateBuffer)load(context, "vkCreateBuffer");
		vkCreateBufferView = (PFN_vkCreateBufferView)load(context, "vkCreateBufferView");
		vkCreateCommandPool = (PFN_vkCreateCommandPool)load(context, "vkCreateCommandPool");
		vkCreateComputePipelines = (PFN_vkCreateComputePipelines)load(context, "vkCreateComputePipelines");
		vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)load(context, "vkCreateDescriptorPool");
		vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)load(context, "vkCreateDescriptorSetLayout");
		vkCreateEvent = (PFN_vkCreateEvent)load(context, "vkCreateEvent");
		vkCreateFence = (PFN_vkCreateFence)load(context, "vkCreateFence");
		vkCreateFramebuffer = (PFN_vkCreateFramebuffer)load(context, "vkCreateFramebuffer");
		vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)load(context, "vkCreateGraphicsPipelines");
		vkCreateImage = (PFN_vkCreateImage)load(context, "vkCreateImage");
		vkCreateImageView = (PFN_vkCreateImageView)load(context, "vkCreateImageView");
		vkCreatePipelineCache = (PFN_vkCreatePipelineCache)load(context, "vkCreatePipelineCache");
		vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)load(context, "vkCreatePipelineLayout");
		vkCreateQueryPool = (PFN_vkCreateQueryPool)load(context, "vkCreateQueryPool");
		vkCreateRenderPass = (PFN_vkCreateRenderPass)load(context, "vkCreateRenderPass");
		vkCreateSampler = (PFN_vkCreateSampler)load(context, "vkCreateSampler");
		vkCreateSemaphore = (PFN_vkCreateSemaphore)load(context, "vkCreateSemaphore");
		vkCreateShaderModule = (PFN_vkCreateShaderModule)load(context, "vkCreateShaderModule");
		vkDestroyBuffer = (PFN_vkDestroyBuffer)load(context, "vkDestroyBuffer");
		vkDestroyBufferView = (PFN_vkDestroyBufferView)load(context, "vkDestroyBufferView");
		vkDestroyCommandPool = (PFN_vkDestroyCommandPool)load(context, "vkDestroyCommandPool");
		vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)load(context, "vkDestroyDescriptorPool");
		vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)load(context, "vkDestroyDescriptorSetLayout");
		vkDestroyDevice = (PFN_vkDestroyDevice)load(context, "vkDestroyDevice");
		vkDestroyEvent = (PFN_vkDestroyEvent)load(context, "vkDestroyEvent");
		vkDestroyFence = (PFN_vkDestroyFence)load(context, "vkDestroyFence");
		vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)load(context, "vkDestroyFramebuffer");
		vkDestroyImage = (PFN_vkDestroyImage)load(context, "vkDestroyImage");
		vkDestroyImageView = (PFN_vkDestroyImageView)load(context, "vkDestroyImageView");
		vkDestroyPipeline = (PFN_vkDestroyPipeline)load(context, "vkDestroyPipeline");
		vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)load(context, "vkDestroyPipelineCache");
		vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)load(context, "vkDestroyPipelineLayout");
		vkDestroyQueryPool = (PFN_vkDestroyQueryPool)load(context, "vkDestroyQueryPool");
		vkDestroyRenderPass = (PFN_vkDestroyRenderPass)load(context, "vkDestroyRenderPass");
		vkDestroySampler = (PFN_vkDestroySampler)load(context, "vkDestroySampler");
		vkDestroySemaphore = (PFN_vkDestroySemaphore)load(context, "vkDestroySemaphore");
		vkDestroyShaderModule = (PFN_vkDestroyShaderModule)load(context, "vkDestroyShaderModule");
		vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)load(context, "vkDeviceWaitIdle");
		vkEndCommandBuffer = (PFN_vkEndCommandBuffer)load(context, "vkEndCommandBuffer");
		vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)load(context, "vkFlushMappedMemoryRanges");
		vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)load(context, "vkFreeCommandBuffers");
		vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)load(context, "vkFreeDescriptorSets");
		vkFreeMemory = (PFN_vkFreeMemory)load(context, "vkFreeMemory");
		vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)load(context, "vkGetBufferMemoryRequirements");
		vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)load(context, "vkGetDeviceMemoryCommitment");
		vkGetDeviceQueue = (PFN_vkGetDeviceQueue)load(context, "vkGetDeviceQueue");
		vkGetEventStatus = (PFN_vkGetEventStatus)load(context, "vkGetEventStatus");
		vkGetFenceStatus = (PFN_vkGetFenceStatus)load(context, "vkGetFenceStatus");
		vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)load(context, "vkGetImageMemoryRequirements");
		vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)load(context, "vkGetImageSparseMemoryRequirements");
		vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)load(context, "vkGetImageSubresourceLayout");
		vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)load(context, "vkGetPipelineCacheData");
		vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)load(context, "vkGetQueryPoolResults");
		vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)load(context, "vkGetRenderAreaGranularity");
		vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)load(context, "vkInvalidateMappedMemoryRanges");
		vkMapMemory = (PFN_vkMapMemory)load(context, "vkMapMemory");
		vkMergePipelineCaches = (PFN_vkMergePipelineCaches)load(context, "vkMergePipelineCaches");
		vkQueueBindSparse = (PFN_vkQueueBindSparse)load(context, "vkQueueBindSparse");
		vkQueueSubmit = (PFN_vkQueueSubmit)load(context, "vkQueueSubmit");
		vkQueueWaitIdle = (PFN_vkQueueWaitIdle)load(context, "vkQueueWaitIdle");
		vkResetCommandBuffer = (PFN_vkResetCommandBuffer)load(context, "vkResetCommandBuffer");
		vkResetCommandPool = (PFN_vkResetCommandPool)load(context, "vkResetCommandPool");
		vkResetDescriptorPool = (PFN_vkResetDescriptorPool)load(context, "vkResetDescriptorPool");
		vkResetEvent = (PFN_vkResetEvent)load(context, "vkResetEvent");
		vkResetFences = (PFN_vkResetFences)load(context, "vkResetFences");
		vkSetEvent = (PFN_vkSetEvent)load(context, "vkSetEvent");
		vkUnmapMemory = (PFN_vkUnmapMemory)load(context, "vkUnmapMemory");
		vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)load(context, "vkUpdateDescriptorSets");
		vkWaitForFences = (PFN_vkWaitForFences)load(context, "vkWaitForFences");
	#endif /* defined(VK_VERSION_1_0) */
	kbhDebugLog("Vulkan RHI : vulkan device function loaded");
}

#if defined(VK_VERSION_1_0)
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	PFN_vkAllocateMemory vkAllocateMemory;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	PFN_vkBindBufferMemory vkBindBufferMemory;
	PFN_vkBindImageMemory vkBindImageMemory;
	PFN_vkCmdBeginQuery vkCmdBeginQuery;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	PFN_vkCmdBindPipeline vkCmdBindPipeline;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	PFN_vkCmdBlitImage vkCmdBlitImage;
	PFN_vkCmdClearAttachments vkCmdClearAttachments;
	PFN_vkCmdClearColorImage vkCmdClearColorImage;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	PFN_vkCmdCopyImage vkCmdCopyImage;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	PFN_vkCmdDispatch vkCmdDispatch;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
	PFN_vkCmdDraw vkCmdDraw;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	PFN_vkCmdEndQuery vkCmdEndQuery;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	PFN_vkCmdFillBuffer vkCmdFillBuffer;
	PFN_vkCmdNextSubpass vkCmdNextSubpass;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	PFN_vkCmdPushConstants vkCmdPushConstants;
	PFN_vkCmdResetEvent vkCmdResetEvent;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	PFN_vkCmdResolveImage vkCmdResolveImage;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
	PFN_vkCmdSetEvent vkCmdSetEvent;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	PFN_vkCmdSetScissor vkCmdSetScissor;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
	PFN_vkCmdSetViewport vkCmdSetViewport;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
	PFN_vkCmdWaitEvents vkCmdWaitEvents;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
	PFN_vkCreateBuffer vkCreateBuffer;
	PFN_vkCreateBufferView vkCreateBufferView;
	PFN_vkCreateCommandPool vkCreateCommandPool;
	PFN_vkCreateComputePipelines vkCreateComputePipelines;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkCreateEvent vkCreateEvent;
	PFN_vkCreateFence vkCreateFence;
	PFN_vkCreateFramebuffer vkCreateFramebuffer;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	PFN_vkCreateImage vkCreateImage;
	PFN_vkCreateImageView vkCreateImageView;
	PFN_vkCreateInstance vkCreateInstance;
	PFN_vkCreatePipelineCache vkCreatePipelineCache;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	PFN_vkCreateQueryPool vkCreateQueryPool;
	PFN_vkCreateRenderPass vkCreateRenderPass;
	PFN_vkCreateSampler vkCreateSampler;
	PFN_vkCreateSemaphore vkCreateSemaphore;
	PFN_vkCreateShaderModule vkCreateShaderModule;
	PFN_vkDestroyBuffer vkDestroyBuffer;
	PFN_vkDestroyBufferView vkDestroyBufferView;
	PFN_vkDestroyCommandPool vkDestroyCommandPool;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyEvent vkDestroyEvent;
	PFN_vkDestroyFence vkDestroyFence;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	PFN_vkDestroyImage vkDestroyImage;
	PFN_vkDestroyImageView vkDestroyImageView;
	PFN_vkDestroyInstance vkDestroyInstance;
	PFN_vkDestroyPipeline vkDestroyPipeline;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	PFN_vkDestroyQueryPool vkDestroyQueryPool;
	PFN_vkDestroyRenderPass vkDestroyRenderPass;
	PFN_vkDestroySampler vkDestroySampler;
	PFN_vkDestroySemaphore vkDestroySemaphore;
	PFN_vkDestroyShaderModule vkDestroyShaderModule;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkEndCommandBuffer vkEndCommandBuffer;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
	PFN_vkFreeMemory vkFreeMemory;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkGetEventStatus vkGetEventStatus;
	PFN_vkGetFenceStatus vkGetFenceStatus;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
	PFN_vkMapMemory vkMapMemory;
	PFN_vkMergePipelineCaches vkMergePipelineCaches;
	PFN_vkQueueBindSparse vkQueueBindSparse;
	PFN_vkQueueSubmit vkQueueSubmit;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
	PFN_vkResetCommandBuffer vkResetCommandBuffer;
	PFN_vkResetCommandPool vkResetCommandPool;
	PFN_vkResetDescriptorPool vkResetDescriptorPool;
	PFN_vkResetEvent vkResetEvent;
	PFN_vkResetFences vkResetFences;
	PFN_vkSetEvent vkSetEvent;
	PFN_vkUnmapMemory vkUnmapMemory;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
	PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
