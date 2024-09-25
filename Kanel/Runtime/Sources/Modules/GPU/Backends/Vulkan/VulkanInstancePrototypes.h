// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

// No header guard

#ifndef KANEL_CLI_VULKAN_INSTANCE_FUNCTION
	#error "You must define KANEL_CLI_VULKAN_INSTANCE_FUNCTION before including this file"
#endif

#ifdef VK_VERSION_1_0
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkCreateDevice)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkDestroyInstance)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetDeviceProcAddr)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceImageFormatProperties)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)
	KANEL_CLI_VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)
#endif
