// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE_INTERNAL
#define KANEL_CLI_VULKAN_CORE_INTERNAL

#include <vulkan/vulkan_core.h>

typedef struct KbhVulkanPFNs
{
	#define KANEL_CLI_VULKAN_GLOBAL_FUNCTION(func) PFN_##func func;
	#include <Modules/GPU/Backends/Vulkan/VulkanGlobalPrototypes.h>
	#undef KANEL_CLI_VULKAN_GLOBAL_FUNCTION

	#define KANEL_CLI_VULKAN_INSTANCE_FUNCTION(func) PFN_##func func;
	#include <Modules/GPU/Backends/Vulkan/VulkanInstancePrototypes.h>
	#undef KANEL_CLI_VULKAN_INSTANCE_FUNCTION
} KbhVulkanPFNs;

KbhVulkanPFNs* kbhGetVulkanPFNs();

#endif
