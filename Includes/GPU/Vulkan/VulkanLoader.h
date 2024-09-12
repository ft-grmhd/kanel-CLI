// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_LOADER
#define KANEL_CLI_VULKAN_LOADER

#include <GPU/Vulkan/VulkanDefs.h>
#include <GPU/Vulkan/VulkanPrototypes.h>

#include <vulkan/vulkan.h>

VkResult kbhVulkanLoaderInit();
void kbhLoadInstance(VkInstance instance);
void kbhVulkanLoaderUninit();

#endif
