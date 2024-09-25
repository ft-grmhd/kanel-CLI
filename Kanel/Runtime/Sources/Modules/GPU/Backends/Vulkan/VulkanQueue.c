// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Backends/Vulkan/VulkanQueue.h>
#include <Modules/GPU/Backends/Vulkan/VulkanDevice.h>
#include <Modules/GPU/Backends/Vulkan/VulkanCoreInternal.h>

#include <Modules/GPU/RHI/RHICore.h>

#include <Core/Logs.h>

#include <stdlib.h>

KbhRHIResult kbhFindPhysicalDeviceQueueFamily(VkPhysicalDevice physical, KbhVulkanQueueType type, int32_t* queue_family_index)
{
	if(physical == VK_NULL_HANDLE)
		return KBH_RHI_ERROR_INVALID_HANDLE;

	uint32_t queue_family_count;
	kbhGetVulkanPFNs()->vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, KANEL_CLI_NULLPTR);
	VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
	if(!queue_families)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	kbhGetVulkanPFNs()->vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, queue_families);

	bool found = false;
	for(int32_t i = 0; i < queue_family_count; i++)
	{
		if(type == KBH_VULKAN_QUEUE_COMPUTE)
		{
			// try to find a queue that's only for compute
			if(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT && (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) == 0)
			{
				*queue_family_index = i;
				found = true;
				break;
			}
			if(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) // else just find a compute queue
			{
				*queue_family_index = i;
				found = true;
				break;
			}
		}
		if(type == KBH_VULKAN_QUEUE_TRANSFER)
		{
			if(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT && (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
			{
				*queue_family_index = i;
				found = true;
				break;
			}
			if(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				*queue_family_index = i;
				found = true;
				break;
			}
		}
	}
	free(queue_families);
	return (found ? KBH_RHI_SUCCESS : KBH_RHI_INCOMPLETE);
}

KbhRHIResult kbhPrepareDeviceQueue(KbhVulkanDevice device, KbhVulkanQueueType type)
{
	if(device == KANEL_CLI_VULKAN_NULL_HANDLE)
		return KBH_RHI_ERROR_INVALID_HANDLE;

	device->queues[(int)type] = (KbhVulkanQueue)malloc(sizeof(KbhVulkanQueueHandler));
	if(!device->queues[(int)type])
		return KBH_RHI_ERROR_ALLOCATION_FAILED;
	KbhVulkanQueue queue = device->queues[(int)type];
	if(!queue)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	if(kbhFindPhysicalDeviceQueueFamily(device->physical, type, &queue->queue_family_index) != KBH_RHI_SUCCESS)
		return KBH_RHI_INCOMPLETE;
	return KBH_RHI_SUCCESS;
}

KbhRHIResult kbhRetrieveDeviceQueue(KbhVulkanDevice device, KbhVulkanQueueType type)
{
	if(device == KANEL_CLI_VULKAN_NULL_HANDLE)
		return KBH_RHI_ERROR_INVALID_HANDLE;
	KbhVulkanQueue queue = device->queues[(int)type];
	if(!queue)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	device->vkGetDeviceQueue(device->device, queue->queue_family_index, 0, &queue->queue);
	return KBH_RHI_SUCCESS;
}
