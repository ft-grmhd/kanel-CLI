// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Vulkan/VulkanCore.h>
#include <Modules/GPU/Vulkan/VulkanLoader.h>
#include <Modules/GPU/Vulkan/VulkanInstance.h>
#include <Modules/GPU/Vulkan/VulkanDevice.h>

#include <Modules/GPU/RHI/RHICore.h>

#include <Config.h>

#include <Core/Logs.h>

#include <stdlib.h>

KbhRHIResult kbhVulkanInit(KbhVulkanContext* context)
{
	*context = (KbhVulkanContext)malloc(sizeof(KbhVulkanContextHandler));
	if(!*context)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	if(kbhVulkanLoaderInit() != VK_SUCCESS)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	const char* instance_extensions[] = { VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME };
	(*context)->instance = kbhVulkanCreateInstance(instance_extensions, sizeof(instance_extensions) / sizeof(const char*));
	kbhLoadInstance((*context)->instance);

	kbhCheckRHI(kbhVulkanLoadNewDevice(*context));

	return KBH_RHI_SUCCESS;
}

KbhRHIResult kbhVulkanLoadNewDevice(KbhVulkanContext context)
{
	if(context == KANEL_CLI_VULKAN_NULL_HANDLE)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	context->devices = (KbhVulkanDevice*)realloc(context->devices, sizeof(KbhVulkanDeviceHandler) * (context->devices_count + 1));
	kbhVerify(context->devices != KANEL_CLI_NULLPTR); // Verify and not return error because if alloc fails, older devices are lost
	kbhCheckRHI(kbhCreateVulkanDevice(context, &context->devices[context->devices_count]));
	context->devices_count++;
	return KBH_RHI_SUCCESS;
}

void kbhVulkanUninit(KbhVulkanContext context)
{
	if(context == KANEL_CLI_VULKAN_NULL_HANDLE)
		return;
	#pragma omp parallel for
	for(size_t i = 0; i < context->devices_count; i++)
		kbhDestroyVulkanDevice(context->devices[i]);
	kbhVulkanDestroyInstance(context->instance);
	kbhVulkanLoaderUninit();
	free(context);
}

static const char* kbhGetBuildVersion()
{
	return KANEL_CLI_VERSION;
}

KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs()
{
	KbhRHILoaderPFNs loader = { 0 };
	loader.f_kbhRHIBackendInitContext = (PFN_kbhRHIBackendInitContext)&kbhVulkanInit;
	loader.f_kbhRHIBackendUninitContext = (PFN_kbhRHIBackendUninitContext)&kbhVulkanUninit;
	loader.f_kbhRHIBackendGetBuildVersion = (PFN_kbhRHIBackendGetBuildVersion)&kbhGetBuildVersion;
	return loader;
}
