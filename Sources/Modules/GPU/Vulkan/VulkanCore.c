// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Vulkan/VulkanCore.h>
#include <Modules/GPU/Vulkan/VulkanLoader.h>

#include <Core/Logs.h>

#define KVF_ASSERT(cond) kbhAssert(cond)
#define KVF_IMPLEMENTATION
#ifdef KANEL_CLI_DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

void kbhErrorCallback(const char* message)
{
	kbhFatalError(message);
	putchar('\n');
}

void kbhValidationErrorCallback(const char* message)
{
	kbhError(message);
	putchar('\n');
}

void kbhValidationWarningCallback(const char* message)
{
	kbhWarning(message);
	putchar('\n');
}

static KbhRHIResult kbhPickPhysicalDevice(VkInstance instance, VkPhysicalDevice* device);

KbhRHIResult kbhVulkanInit(KbhVulkanContext** context)
{
	*context = (KbhVulkanContext*)malloc(sizeof(KbhVulkanContext));
	if(!*context)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	if(kbhVulkanLoaderInit() != VK_SUCCESS)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;

	kvfSetErrorCallback(&kbhErrorCallback);
	kvfSetValidationErrorCallback(&kbhValidationErrorCallback);
	kvfSetValidationWarningCallback(&kbhValidationWarningCallback);

	(*context)->instance = kvfCreateInstance(KANEL_CLI_NULLPTR, 0);
	kbhDebugLog("Vulkan : instance created");
	kbhLoadInstance((*context)->instance);

	if(kbhPickPhysicalDevice((*context)->instance, &(*context)->physical_device) != KBH_RHI_SUCCESS)
	{
		kbhError("Vulkan : could not find a suitable physical device");
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	}
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties((*context)->physical_device, &props);
	kbhDebugLogFmt("Vulkan : physical device picked, '%s'", props.deviceName);

	VkPhysicalDeviceFeatures device_features = { VK_FALSE };
	int32_t queue = kvfFindDeviceQueueFamily((*context)->physical_device, KVF_COMPUTE_QUEUE);
	(*context)->device = kvfCreateDeviceCustomPhysicalDeviceAndQueues((*context)->physical_device, KANEL_CLI_NULLPTR, 0, &device_features, -1, -1, queue);
	kbhDebugLog("Vulkan : logical device created");

	return KBH_RHI_SUCCESS;
}

void kbhVulkanUninit(KbhVulkanContext* context)
{
	vkDeviceWaitIdle(context->device);

	kvfDestroyDevice(context->device);
	kbhDebugLog("Vulkan : instance destroyed");
	kvfDestroyInstance(context->instance);
	kbhDebugLog("Vulkan : logical device destroyed");
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

static int32_t kbhScorePhysicalDevice(VkPhysicalDevice device, const char** device_extensions, uint32_t device_extensions_count)
{
	/* Check extensions support */
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);
	VkExtensionProperties* props = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extension_count + 1);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, props);

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
	int32_t queue = kvfFindDeviceQueueFamily(device, KVF_COMPUTE_QUEUE);
	if(queue == -1)
		return -1;

	VkPhysicalDeviceProperties device_props;
	vkGetPhysicalDeviceProperties(device, &device_props);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

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
	VkPhysicalDevice* devices = NULL;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;
	uint32_t device_count;
	int32_t best_device_score = -1;

	kbhAssert(instance != VK_NULL_HANDLE);
	kbhAssert(device != KANEL_CLI_NULLPTR);

	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);

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
