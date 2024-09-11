// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/Vulkan/VulkanCore.h>
#include <GPU/Vulkan/VulkanLoader.h>

#define KVF_IMPLEMENTATION
#ifdef KANEL_CLI_DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

KbhRHIResult kbhVulkanInit(KbhVulkanCore* core) KANEL_CLI_NONNULL(1)
{
	if(kbhVulkanLoaderInit() != VK_SUCCESS)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	core->instance = kvfCreateInstance(KANEL_CLI_NULLPTR, 0);
	kbhLoadInstance(core->instance);

	core->physical_device = kvfPickGoodDefaultPhysicalDevice(core->instance, VK_NULL_HANDLE);

	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(core->physical_device, &props);

	VkPhysicalDeviceFeatures features{};
	vkGetPhysicalDeviceFeatures(core->physical_device, &features);
	core->device = kvfCreateDevice(core->physical_device, KANEL_CLI_NULLPTR, 0, &features);

	return KBH_RHI_SUCCESS;
}

void kbhVulkanUninit(KbhVulkanCore* core) KANEL_CLI_NONNULL(1)
{
	vkDeviceWaitIdle(core->device);

	kvfDestroyDevice(core->device);
	kvfDestroyInstance(core->instance);
	kbhVulkanLoaderUninit();
}
