// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_DEVICE
#define KANEL_CLI_VULKAN_DEVICE

#include <Modules/GPU/Backends/Vulkan/VulkanCore.h>
#include <Modules/GPU/Backends/Vulkan/VulkanQueue.h>
#include <Modules/GPU/Backends/Vulkan/VulkanInternalEnums.h>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1000000
#include <vk_mem_alloc.h>

KANEL_CLI_VULKAN_DEFINE_NULLABLE_HANDLE(KbhVulkanDevice);

typedef struct KbhVulkanDeviceHandler
{
	KbhVulkanQueue queues[KBH_VULKAN_QUEUE_END_ENUM];

	VkPhysicalDeviceFeatures features;
	VkPhysicalDeviceMemoryProperties memory_properties;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDevice physical;

	VkDevice device;

	VmaAllocator allocator;

	#define KANEL_CLI_VULKAN_DEVICE_FUNCTION(func) PFN_##func func;
		#include <Modules/GPU/Backends/Vulkan/VulkanDevicePrototypes.h>
	#undef KANEL_CLI_VULKAN_DEVICE_FUNCTION
} KbhVulkanDeviceHandler;

KANEL_CLI_NONNULL(2) KbhRHIResult kbhCreateVulkanDevice(KbhVulkanContext context, KbhVulkanDevice* device);
void kbhDestroyVulkanDevice(KbhVulkanDevice device);

#endif
