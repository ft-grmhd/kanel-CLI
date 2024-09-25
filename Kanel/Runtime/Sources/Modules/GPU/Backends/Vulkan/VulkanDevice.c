// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Backends/Vulkan/VulkanDevice.h>
#include <Modules/GPU/Backends/Vulkan/VulkanCoreInternal.h>
#include <Modules/GPU/Backends/Vulkan/VulkanQueue.h>
#include <Modules/GPU/Backends/Vulkan/VulkanLoader.h>

#include <Modules/GPU/RHI/RHICore.h>

#include <Core/Logs.h>

#include <string.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

static int32_t kbhScorePhysicalDevice(VkPhysicalDevice device, const char** device_extensions, uint32_t device_extensions_count)
{
	/* Check extensions support */
	uint32_t extension_count;
	kbhGetVulkanPFNs()->vkEnumerateDeviceExtensionProperties(device, KANEL_CLI_NULLPTR, &extension_count, KANEL_CLI_NULLPTR);
	VkExtensionProperties* props = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extension_count + 1);
	if(!props)
		return -1;
	kbhGetVulkanPFNs()->vkEnumerateDeviceExtensionProperties(device, KANEL_CLI_NULLPTR, &extension_count, props);

	bool are_there_required_device_extensions = true;
	for(int j = 0; j < device_extensions_count; j++)
	{
		bool is_there_extension = false;
		for(int k = 0; k < extension_count; k++)
		{
			if(strcmp(device_extensions[j], props[k].extensionName) == 0)
			{
				is_there_extension = true;
				break;
			}
		}
		if(is_there_extension == false)
		{
			are_there_required_device_extensions = false;
			break;
		}
	}
	free(props);
	if(are_there_required_device_extensions == false)
		return -1;

	/* Check Queue Families Support */
	int32_t queue;
	if(kbhFindPhysicalDeviceQueueFamily(device, KBH_VULKAN_QUEUE_COMPUTE, &queue) != KBH_RHI_SUCCESS)
		return -1;

	VkPhysicalDeviceProperties device_props;
	kbhGetVulkanPFNs()->vkGetPhysicalDeviceProperties(device, &device_props);

	VkPhysicalDeviceFeatures device_features;
	kbhGetVulkanPFNs()->vkGetPhysicalDeviceFeatures(device, &device_features);

	int32_t score = -1;
	if(device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1001;

	score += device_props.limits.maxComputeWorkGroupCount[0];
	score += device_props.limits.maxComputeWorkGroupCount[1];
	score += device_props.limits.maxComputeWorkGroupCount[2];
	score += device_props.limits.maxComputeSharedMemorySize;

	return score;
}

static KbhRHIResult kbhPickPhysicalDevice(VkInstance instance, VkPhysicalDevice* device)
{
	VkPhysicalDevice* devices = KANEL_CLI_NULLPTR;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;
	uint32_t device_count;
	int32_t best_device_score = -1;

	kbhAssert(instance != VK_NULL_HANDLE);
	kbhAssert(device != KANEL_CLI_NULLPTR);

	kbhGetVulkanPFNs()->vkEnumeratePhysicalDevices(instance, &device_count, KANEL_CLI_NULLPTR);
	devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * device_count + 1);
	if(!devices)
		return KBH_RHI_ERROR_ALLOCATION_FAILED;
	kbhGetVulkanPFNs()->vkEnumeratePhysicalDevices(instance, &device_count, devices);

	for(int i = 0; i < device_count; i++)
	{
		int32_t current_device_score = kbhScorePhysicalDevice(devices[i], KANEL_CLI_NULLPTR, 0);
		if(current_device_score > best_device_score)
		{
			best_device_score = current_device_score;
			chosen_one = devices[i];
		}
	}
	free(devices);
	if(chosen_one != VK_NULL_HANDLE)
	{
		*device = chosen_one;
		return KBH_RHI_SUCCESS;
	}
	return KBH_RHI_ERROR_INITIALIZATION_FAILED;
}

KbhRHIResult kbhCreateVulkanDevice(KbhVulkanContext context, KbhVulkanDevice* device)
{
	if(context == KANEL_CLI_VULKAN_NULL_HANDLE)
		return KBH_RHI_ERROR_INVALID_HANDLE;

	*device = (KbhVulkanDevice)calloc(1, sizeof(KbhVulkanDeviceHandler));
	if(!*device)
		return KBH_RHI_ERROR_ALLOCATION_FAILED;

	kbhCheckRHI(kbhPickPhysicalDevice(context->instance, &(*device)->physical));

	const float queue_priority = 1.0f;

	VkDeviceQueueCreateInfo* queue_create_infos = (VkDeviceQueueCreateInfo*)calloc(1, KBH_VULKAN_QUEUE_END_ENUM * sizeof(VkDeviceQueueCreateInfo));
	if(!queue_create_infos)
		return KBH_RHI_ERROR_ALLOCATION_FAILED;

	uint32_t unique_queues_count = 1;

	#pragma omp parallel for
	for(int i = 0; i < KBH_VULKAN_QUEUE_END_ENUM; i++) // Needs to be done before next loop
		kbhCheckRHI(kbhPrepareDeviceQueue(*device, i));

	#pragma omp parallel for
	for(int i = 0; i < KBH_VULKAN_QUEUE_END_ENUM; i++)
	{
		if((*device)->queues[i]->queue_family_index == -1)
			continue;
		int j;
		for(j = i; j < KBH_VULKAN_QUEUE_END_ENUM; j++) // Ugly shit but array will never be big so it's okay
		{
			if((*device)->queues[i]->queue_family_index == (*device)->queues[j]->queue_family_index)
				break;
		}
		if(j == KBH_VULKAN_QUEUE_END_ENUM)
			unique_queues_count++;
		queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_infos[i].queueFamilyIndex = (*device)->queues[i]->queue_family_index;
		queue_create_infos[i].queueCount = 1;
		queue_create_infos[i].pQueuePriorities = &queue_priority;
		queue_create_infos[i].flags = 0;
		queue_create_infos[i].pNext = KANEL_CLI_NULLPTR;
	}

	VkDeviceCreateInfo create_info;
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.queueCreateInfoCount = unique_queues_count;
	create_info.pQueueCreateInfos = queue_create_infos;
	create_info.pEnabledFeatures = &(*device)->features;
	create_info.enabledExtensionCount = 0;
	create_info.ppEnabledExtensionNames = KANEL_CLI_NULLPTR;
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = KANEL_CLI_NULLPTR;
	create_info.flags = 0;
	create_info.pNext = KANEL_CLI_NULLPTR;

	kbhCheckVk(kbhGetVulkanPFNs()->vkCreateDevice((*device)->physical, &create_info, KANEL_CLI_NULLPTR, &(*device)->device));
	kbhLoadDevice(*device);
	for(int i = 0; i < KBH_VULKAN_QUEUE_END_ENUM; i++)
		kbhCheckRHI(kbhRetrieveDeviceQueue(*device, i));
	kbhDebugLog("Vulkan : retrieved device queues");

	VmaVulkanFunctions vma_vulkan_func = {};
	vma_vulkan_func.vkAllocateMemory                    = (*device)->vkAllocateMemory;
	vma_vulkan_func.vkBindBufferMemory                  = (*device)->vkBindBufferMemory;
	vma_vulkan_func.vkBindImageMemory                   = (*device)->vkBindImageMemory;
	vma_vulkan_func.vkCreateBuffer                      = (*device)->vkCreateBuffer;
	vma_vulkan_func.vkCreateImage                       = (*device)->vkCreateImage;
	vma_vulkan_func.vkDestroyBuffer                     = (*device)->vkDestroyBuffer;
	vma_vulkan_func.vkDestroyImage                      = (*device)->vkDestroyImage;
	vma_vulkan_func.vkFlushMappedMemoryRanges           = (*device)->vkFlushMappedMemoryRanges;
	vma_vulkan_func.vkFreeMemory                        = (*device)->vkFreeMemory;
	vma_vulkan_func.vkGetBufferMemoryRequirements       = (*device)->vkGetBufferMemoryRequirements;
	vma_vulkan_func.vkGetImageMemoryRequirements        = (*device)->vkGetImageMemoryRequirements;
	vma_vulkan_func.vkInvalidateMappedMemoryRanges      = (*device)->vkInvalidateMappedMemoryRanges;
	vma_vulkan_func.vkMapMemory                         = (*device)->vkMapMemory;
	vma_vulkan_func.vkUnmapMemory                       = (*device)->vkUnmapMemory;
	vma_vulkan_func.vkCmdCopyBuffer                     = (*device)->vkCmdCopyBuffer;
	vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties = kbhGetVulkanPFNs()->vkGetPhysicalDeviceMemoryProperties;
	vma_vulkan_func.vkGetPhysicalDeviceProperties       = kbhGetVulkanPFNs()->vkGetPhysicalDeviceProperties;

	VmaAllocatorCreateInfo allocator_create_info = {};
	allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_0;
	allocator_create_info.physicalDevice = (*device)->physical;
	allocator_create_info.device = (*device)->device;
	allocator_create_info.instance = context->instance;
	allocator_create_info.pVulkanFunctions = &vma_vulkan_func;

	kbhCheckVk(vmaCreateAllocator(&allocator_create_info, &(*device)->allocator));

	kbhDebugLogFmt("Vulkan : logical device created from %s", (*device)->properties.deviceName);
	return KBH_RHI_SUCCESS;
}

void kbhDestroyVulkanDevice(KbhVulkanDevice device)
{
	if(device == KANEL_CLI_VULKAN_NULL_HANDLE)
		return;
	vmaDestroyAllocator(device->allocator);
	device->vkDestroyDevice(device->device, KANEL_CLI_NULLPTR);
	kbhDebugLog("Vulkan : logical device destroyed");
}
