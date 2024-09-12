// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/Vulkan/VulkanCore.h>
#include <GPU/Vulkan/VulkanLoader.h>

#include <Core/Logs.h>

#define KVF_IMPLEMENTATION
#ifdef KANEL_CLI_DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

KbhRHIResult kbhVulkanInit(KbhVulkanContext** context)
{
	*context = (KbhVulkanContext*)malloc(sizeof(KbhVulkanContext));
	if(!*context)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	if(kbhVulkanLoaderInit() != VK_SUCCESS)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	(*context)->instance = kvfCreateInstance(KANEL_CLI_NULLPTR, 0);
	kbhDebugLog("Vulkan RHI : instance created");
	kbhLoadInstance((*context)->instance);

	(*context)->physical_device = kvfPickGoodDefaultPhysicalDevice((*context)->instance, VK_NULL_HANDLE);
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties((*context)->physical_device, &props);
	kbhDebugLogFmt("Vulkan RHI : physical device picked, '%s'", props.deviceName);

	VkPhysicalDeviceFeatures features = {};
	vkGetPhysicalDeviceFeatures((*context)->physical_device, &features);
	(*context)->device = kvfCreateDevice((*context)->physical_device, KANEL_CLI_NULLPTR, 0, &features);
	kbhDebugLog("Vulkan RHI : logical device created");

	return KBH_RHI_SUCCESS;
}

void kbhVulkanUninit(KbhVulkanContext* context)
{
	vkDeviceWaitIdle(context->device);

	kvfDestroyDevice(context->device);
	kbhDebugLog("Vulkan RHI : instance destroyed");
	kvfDestroyInstance(context->instance);
	kbhDebugLog("Vulkan RHI : logical device destroyed");
	kbhVulkanLoaderUninit();

	free(context);
}

KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs()
{
	KbhRHILoaderPFNs loader = { 0 };
	loader.f_kbhRHIBackendInitContext = (PFN_kbhRHIBackendInitContext)&kbhVulkanInit;
	loader.f_kbhRHIBackendUninitContext = (PFN_kbhRHIBackendUninitContext)&kbhVulkanUninit;
	return loader;
}
