// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE
#define KANEL_CLI_VULKAN_CORE

#include <GPU/Vulkan/VulkanDefs.h>
#include <GPU/Vulkan/VulkanPrototypes.h>

#include <GPU/RHI/RHILoader.h>

#include <GPU/RHI/RHIEnums.h>

typedef struct KbhVulkanContext
{
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
} KbhVulkanContext;

KANEL_CLI_VULKAN_API KbhRHIResult kbhVulkanInit(KbhVulkanContext** context) KANEL_CLI_NONNULL(1);
KANEL_CLI_VULKAN_API void kbhVulkanUninit(KbhVulkanContext* context) KANEL_CLI_NONNULL(1);

KANEL_CLI_VULKAN_API KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs();

#endif
