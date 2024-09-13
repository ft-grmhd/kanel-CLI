// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE
#define KANEL_CLI_VULKAN_CORE

#include <Modules/GPU/Vulkan/VulkanDefs.h>
#include <Modules/GPU/Vulkan/VulkanPrototypes.h>

#include <Modules/GPU/RHI/RHILoader.h>

#include <Modules/GPU/RHI/RHIEnums.h>

typedef struct KbhVulkanContext
{
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
} KbhVulkanContext;

KANEL_CLI_NONNULL(1) KbhRHIResult kbhVulkanInit(KbhVulkanContext** context);
void kbhVulkanUninit(KbhVulkanContext* context);

KANEL_CLI_VULKAN_API KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs();

#endif
