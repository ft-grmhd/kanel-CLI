// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_LOADER
#define KANEL_CLI_VULKAN_LOADER

#include <Modules/GPU/Vulkan/VulkanDefs.h>
#include <Modules/GPU/Vulkan/VulkanPrototypes.h>
#include <Modules/GPU/Vulkan/VulkanDevice.h>

#include <vulkan/vulkan.h>

VkResult kbhVulkanLoaderInit();
void kbhLoadInstance(VkInstance instance);
void kbhLoadDevice(KbhVulkanDevice device);
void kbhVulkanLoaderUninit();

#endif
