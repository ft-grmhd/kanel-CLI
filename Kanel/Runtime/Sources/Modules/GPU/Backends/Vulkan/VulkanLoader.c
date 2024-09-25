// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Backends/Vulkan/VulkanLoader.h>

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
	PFN_vkVoidFunction fn = kbhGetVulkanPFNs()->vkGetInstanceProcAddr((VkInstance)context, name);
	if(!fn)
		kbhFatalErrorFmt("Vulkan loader: could not load %s", name);
	return fn;
}

static inline PFN_vkVoidFunction vkGetDeviceProcAddrStub(void* context, const char* name)
{
	PFN_vkVoidFunction fn = kbhGetVulkanPFNs()->vkGetDeviceProcAddr((VkDevice)context, name);
	if(!fn)
		kbhFatalErrorFmt("Vulkan loader: could not load %s", name);
	return fn;
}

VkResult kbhVulkanLoaderInit()
{
	#if defined(KANEL_CLI_PLAT_WINDOWS)
		const char* libnames[] = {
			"vulkan-1.dll"
		};
	#elif defined(KANEL_CLI_PLAT_MACOS)
		const char* libnames[] = {
			"libvulkan.dylib",
			"libvulkan.1.dylib",
			"libMoltenVK.dylib",
			"vulkan.framework/vulkan",
			"MoltenVK.framework/MoltenVK",
			"/usr/local/lib/libvulkan.dylib",
		};
	#else
		const char* libnames[] = {
			"libvulkan.so.1",
			"libvulkan.so"
		};
	#endif

	for(size_t i = 0; i < sizeof(libnames) / sizeof(const char*); i++)
	{
		__kbh_vulkan_lib_module = kbhLoadLibrary(libnames[i]);
		if(__kbh_vulkan_lib_module)
		{
			kbhGetVulkanPFNs()->vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)kbhLoadSymbolFromLibModule(__kbh_vulkan_lib_module, "vkGetInstanceProcAddr");
			if(kbhGetVulkanPFNs()->vkGetInstanceProcAddr == KANEL_CLI_NULLPTR)
			{
				kbhUnloadLibrary(__kbh_vulkan_lib_module);
				__kbh_vulkan_lib_module = KBH_NULL_LIB_MODULE;
			}
			else
			{
				kbhDebugLogFmt("Vulkan loader: loaded vulkan lib using %s", libnames[i]);
				break;
			}
		}
	}
	if(!__kbh_vulkan_lib_module)
		return VK_ERROR_INITIALIZATION_FAILED;
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
	kbhDebugLog("Vulkan: vulkan library unloaded");
}

static void kbhVulkanLoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#define KANEL_CLI_VULKAN_GLOBAL_FUNCTION(func) kbhGetVulkanPFNs()->func = (PFN_##func)load(context, #func);
		#include <Modules/GPU/Backends/Vulkan/VulkanGlobalPrototypes.h>
	#undef KANEL_CLI_VULKAN_GLOBAL_FUNCTION

	kbhDebugLog("Vulkan: vulkan global functions loaded");
}

static void kbhVulkanLoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#define KANEL_CLI_VULKAN_INSTANCE_FUNCTION(func) kbhGetVulkanPFNs()->func = (PFN_##func)load(context, #func);
		#include <Modules/GPU/Backends/Vulkan/VulkanInstancePrototypes.h>
	#undef KANEL_CLI_VULKAN_INSTANCE_FUNCTION

	kbhDebugLog("Vulkan: vulkan instance function loaded");
}

static void kbhVulkanLoadDeviceFunctions(KbhVulkanDevice device, PFN_vkVoidFunction (*load)(void*, const char*))
{
	#define KANEL_CLI_VULKAN_DEVICE_FUNCTION(func) device->func = (PFN_##func)load(device->device, #func);
		#include <Modules/GPU/Backends/Vulkan/VulkanDevicePrototypes.h>
	#undef KANEL_CLI_VULKAN_DEVICE_FUNCTION

	kbhDebugLog("Vulkan: vulkan device function loaded");
}
