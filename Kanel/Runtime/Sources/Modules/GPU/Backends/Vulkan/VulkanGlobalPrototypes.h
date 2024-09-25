// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

// No header guard

#ifndef KANEL_CLI_VULKAN_GLOBAL_FUNCTION
	#error "You must define KANEL_CLI_VULKAN_GLOBAL_FUNCTION before including this file"
#endif

#ifdef VK_VERSION_1_0
	KANEL_CLI_VULKAN_GLOBAL_FUNCTION(vkCreateInstance)
	KANEL_CLI_VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)
	KANEL_CLI_VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)
	KANEL_CLI_VULKAN_GLOBAL_FUNCTION(vkGetInstanceProcAddr)
#endif
