// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_QUEUE
#define KANEL_CLI_VULKAN_QUEUE

#include <Modules/GPU/Vulkan/VulkanCore.h>
#include <Modules/GPU/Vulkan/VulkanInternalEnums.h>

#include <stdint.h>

KANEL_CLI_VULKAN_DEFINE_NULLABLE_HANDLE(KbhVulkanQueue);

typedef struct KbhVulkanQueueHandler
{
	KbhVulkanDevice device;
	VkQueue queue;
	int32_t queue_family_index;
} KbhVulkanQueueHandler;

KANEL_CLI_NONNULL(3) KbhRHIResult kbhFindPhysicalDeviceQueueFamily(VkPhysicalDevice physical, KbhVulkanQueueType type, int32_t* queue_family_index);
KbhRHIResult kbhRetrieveDeviceQueue(KbhVulkanDevice device, KbhVulkanQueueType type);

#endif
