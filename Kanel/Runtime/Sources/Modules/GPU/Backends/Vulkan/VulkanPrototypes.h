// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_PROTOTYPES
#define KANEL_CLI_VULKAN_PROTOTYPES

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
	#error define VK_NO_PROTOTYPES needed
#endif

#ifndef VK_NO_PROTOTYPES
	#define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
	#include <vulkan/vulkan.h>
#endif

#ifdef VK_VERSION_1_0
	extern PFN_vkCreateDevice vkCreateDevice;
	extern PFN_vkCreateInstance vkCreateInstance;
	extern PFN_vkDestroyDevice vkDestroyDevice;
	extern PFN_vkDestroyInstance vkDestroyInstance;
	extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
	extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
	extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
#endif

#endif
