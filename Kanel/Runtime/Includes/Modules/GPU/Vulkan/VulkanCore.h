// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE
#define KANEL_CLI_VULKAN_CORE

#include <Modules/GPU/Vulkan/VulkanDefs.h>

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
	#error define VK_NO_PROTOTYPES needed
#endif

#ifndef VK_NO_PROTOTYPES
	#define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
	#include <vulkan/vulkan.h>
#endif

#include <Modules/GPU/RHI/RHILoader.h>
#include <Modules/GPU/RHI/RHIEnums.h>

#include <Core/Logs.h>

typedef struct VulkanDevice VulkanDevice;

typedef struct KbhVulkanContext
{
	VkInstance instance;
	VulkanDevice* devices;
	size_t devices_count;
} KbhVulkanContext;

KANEL_CLI_NONNULL(1) KbhRHIResult kbhVulkanInit(KbhVulkanContext** context);
void kbhVulkanUninit(KbhVulkanContext* context);

KANEL_CLI_VULKAN_API KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs();

static inline const char* kbhVerbaliseVkResult(VkResult result)
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

static inline void kbhCheckVkBackend(VkResult result, const char* file, const char* function, int line)
{
	if(result != KBH_RHI_SUCCESS)
		kbhFatalErrorBackend("Vulkan check failed due to: %s", file, function, line, kbhVerbaliseVkResult(result));
}

#undef kbhCheckVk
#define kbhCheckVk(res) kbhCheckVkBackend(res, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#endif
