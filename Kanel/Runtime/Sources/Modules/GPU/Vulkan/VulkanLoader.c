// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Vulkan/VulkanLoader.h>

#include <Core/LibLoader.h>
#include <Core/Logs.h>


#ifdef KANEL_CLI_PLAT_MACOS
	#include <stdlib.h> // getenv
#endif

static KbhLibModule __kbh_vulkan_lib_module = KBH_NULL_LIB_MODULE;

static void kbhVulkanLoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void kbhVulkanLoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*));
static void kbhVulkanLoadDeviceFunctions(KbhVulkanDevice device, PFN_vkVoidFunction (*load)(void*, const char*));

static inline PFN_vkVoidFunction vkGetInstanceProcAddrStub(void* context, const char* name)
{
	PFN_vkVoidFunction fn = vkGetInstanceProcAddr((VkInstance)context, name);
	if(!fn)
		kbhFatalErrorFmt("Vulkan loader : could not load %s", name);
	return fn;
}

static inline PFN_vkVoidFunction vkGetDeviceProcAddrStub(void* context, const char* name)
{
	PFN_vkVoidFunction fn = vkGetDeviceProcAddr((VkDevice)context, name);
	if(!fn)
		kbhFatalErrorFmt("Vulkan loader : could not load %s", name);
	return fn;
}

VkResult kbhVulkanLoaderInit()
{
	// Yoinked from volk hehe

	#if defined(KANEL_CLI_PLAT_WINDOWS)
		__kbh_vulkan_lib_module = kbhLoadLibrary("vulkan-1.dll");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#elif defined(KANEL_CLI_PLAT_MACOS)
		__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.dylib");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.1.dylib");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libMoltenVK.dylib");

		// Add support for using Vulkan and MoltenVK in a Framework. App store rules for iOS
		// strictly enforce no .dylib's. If they aren't found it just falls through
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("vulkan.framework/vulkan");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("MoltenVK.framework/MoltenVK");

		// modern versions of macOS don't search /usr/local/lib automatically contrary to what man dlopen says
		// Vulkan SDK uses this as the system-wide installation location, so we're going to fallback to this if all else fails
		if(!__kbh_vulkan_lib_module && getenv("DYLD_FALLBACK_LIBRARY_PATH") == NULL)
			__kbh_vulkan_lib_module = kbhLoadLibrary("/usr/local/lib/libvulkan.dylib");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#else
		__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.so.1");
		if(!__kbh_vulkan_lib_module)
			__kbh_vulkan_lib_module = kbhLoadLibrary("libvulkan.so");
		if(!__kbh_vulkan_lib_module)
			return VK_ERROR_INITIALIZATION_FAILED;
	#endif
	vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)kbhLoadSymbolFromLibModule(__kbh_vulkan_lib_module, "vkGetInstanceProcAddr");
	kbhDebugLog("Vulkan : vulkan library loaded");
	kbhVulkanLoadGlobalFunctions(KANEL_CLI_NULLPTR, vkGetInstanceProcAddrStub);

	return VK_SUCCESS;
}

void kbhLoadInstance(VkInstance instance)
{
	kbhVulkanLoadInstanceFunctions(instance, vkGetInstanceProcAddrStub);
}

void kbhLoadDevice(KbhVulkanDevice device)
{
	kbhVulkanLoadDeviceFunctions(device, vkGetDeviceProcAddrStub);
}

void kbhVulkanLoaderUninit()
{
	kbhUnloadLibrary(__kbh_vulkan_lib_module);
	__kbh_vulkan_lib_module = KBH_NULL_LIB_MODULE;
	kbhDebugLog("Vulkan : vulkan library unloaded");
}

static void kbhVulkanLoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#ifdef VK_VERSION_1_0
		vkCreateInstance = (PFN_vkCreateInstance)load(context, "vkCreateInstance");
		vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)load(context, "vkEnumerateInstanceExtensionProperties");
		vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)load(context, "vkEnumerateInstanceLayerProperties");
	#endif
	kbhDebugLog("Vulkan : vulkan global functions loaded");
}

static void kbhVulkanLoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#ifdef VK_VERSION_1_0
		vkCreateDevice = (PFN_vkCreateDevice)load(context, "vkCreateDevice");
		vkDestroyInstance = (PFN_vkDestroyInstance)load(context, "vkDestroyInstance");
		vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)load(context, "vkEnumerateDeviceExtensionProperties");
		vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)load(context, "vkEnumerateDeviceLayerProperties");
		vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)load(context, "vkEnumeratePhysicalDevices");
		vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)load(context, "vkGetDeviceProcAddr");
		vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)load(context, "vkGetPhysicalDeviceFeatures");
		vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)load(context, "vkGetPhysicalDeviceFormatProperties");
		vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)load(context, "vkGetPhysicalDeviceImageFormatProperties");
		vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)load(context, "vkGetPhysicalDeviceMemoryProperties");
		vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)load(context, "vkGetPhysicalDeviceProperties");
		vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)load(context, "vkGetPhysicalDeviceQueueFamilyProperties");
		vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)load(context, "vkGetPhysicalDeviceSparseImageFormatProperties");
	#endif
	kbhDebugLog("Vulkan : vulkan instance function loaded");
}

static void kbhVulkanLoadDeviceFunctions(KbhVulkanDevice device, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#define KANEL_CLI_VULKAN_DEVICE_FUNCTION(func) device->func = (PFN_##func)load(device->device, #func);
	#include <Modules/GPU/Vulkan/VulkanDevicePrototypes.h>
	#undef KANEL_CLI_VULKAN_DEVICE_FUNCTION

	kbhDebugLog("Vulkan : vulkan device function loaded");
}

#ifdef VK_VERSION_1_0
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkCreateInstance vkCreateInstance;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyInstance vkDestroyInstance;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
#endif
