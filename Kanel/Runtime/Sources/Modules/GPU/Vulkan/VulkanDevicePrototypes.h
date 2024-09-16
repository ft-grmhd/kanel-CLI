// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

// No header guard

#ifndef KANEL_CLI_VULKAN_DEVICE_FUNCTION
	#error "You must define KANEL_CLI_VULKAN_DEVICE_FUNCTION before including this file"
#endif

#ifdef VK_VERSION_1_0
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkAllocateCommandBuffers)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkAllocateDescriptorSets)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkAllocateMemory)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkBeginCommandBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkBindBufferMemory)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdBindDescriptorSets)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdBindPipeline)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdCopyBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdDispatch)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdDispatchIndirect)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdExecuteCommands)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdFillBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdPipelineBarrier)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdPushConstants)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCmdUpdateBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateBufferView)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateCommandPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateComputePipelines)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateDescriptorPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateDescriptorSetLayout)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateFence)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreatePipelineCache)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreatePipelineLayout)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateSemaphore)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkCreateShaderModule)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyBufferView)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyCommandPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyDescriptorPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyDevice)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyFence)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyPipeline)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyPipelineCache)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyPipelineLayout)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroySemaphore)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDestroyShaderModule)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkDeviceWaitIdle)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkEndCommandBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkFlushMappedMemoryRanges)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkFreeCommandBuffers)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkFreeMemory)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkGetBufferMemoryRequirements)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkGetDeviceMemoryCommitment)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkGetDeviceQueue)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkGetFenceStatus)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkInvalidateMappedMemoryRanges)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkMapMemory)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkMergePipelineCaches)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkQueueSubmit)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkQueueWaitIdle)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkResetCommandBuffer)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkResetCommandPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkResetDescriptorPool)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkResetFences)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkUnmapMemory)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkUpdateDescriptorSets)
	KANEL_CLI_VULKAN_DEVICE_FUNCTION(vkWaitForFences)
#endif
