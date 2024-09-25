// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Backends/Vulkan/VulkanCore.h>
#include <Modules/GPU/Backends/Vulkan/VulkanLoader.h>
#include <Modules/GPU/Backends/Vulkan/VulkanInstance.h>
#include <Modules/GPU/Backends/Vulkan/VulkanDevice.h>

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
		return KBH_RHI_ERROR_ALLOCATION_FAILED;
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

static const char* kbhGetVulkanBackendBuildVersion()
{
	return KANEL_CLI_VERSION;
}

KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs()
{
	KbhRHILoaderPFNs loader = { 0 };
	loader.f_kbhRHIBackendInitContext = (PFN_kbhRHIBackendInitContext)&kbhVulkanInit;
	loader.f_kbhRHIBackendUninitContext = (PFN_kbhRHIBackendUninitContext)&kbhVulkanUninit;
	loader.f_kbhRHIBackendGetBuildVersion = (PFN_kbhRHIBackendGetBuildVersion)&kbhGetVulkanBackendBuildVersion;
	return loader;
}

const char* kbhVerbaliseVkResult(VkResult result)
{
	switch(result)
	{
		case VK_SUCCESS: return "Success";
		case VK_NOT_READY: return "A fence or query has not yet completed";
		case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
		case VK_EVENT_SET: return "An event is signaled";
		case VK_EVENT_RESET: return "An event is unsignaled";
		case VK_INCOMPLETE: return "A return array was too small for the result";
		case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
		case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
		case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
		case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
		case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
		case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
		case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
		case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
		case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
		case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
		case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
		case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
		case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";

		default: return "Unknown Vulkan error";
	}
	return KANEL_CLI_NULLPTR; // just to avoid warnings
}

// Internal Vulkan Core

#include <Modules/GPU/Backends/Vulkan/VulkanCoreInternal.h>

KbhVulkanPFNs* kbhGetVulkanPFNs()
{
	static KbhVulkanPFNs pfns;
	return &pfns;
}
