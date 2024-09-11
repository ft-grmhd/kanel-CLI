// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE
#define KANEL_CLI_VULKAN_CORE

#include <GPU/Vulkan/VulkanDefs.h>
#include <GPU/Vulkan/VulkanPrototypes.h>

#include <GPU/RHI/RHIEnums.h>

KANEL_CLI_VULKAN_API typedef struct KbhVulkanCore
{
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
} KbhVulkanCore;

KANEL_CLI_VULKAN_API [[nodiscard]] KbhRHIResult kbhVulkanInit(KbhVulkanCore* core) KANEL_CLI_NONNULL(1);
KANEL_CLI_VULKAN_API void kbhVulkanUninit(KbhVulkanCore* core) KANEL_CLI_NONNULL(1);

#endif
