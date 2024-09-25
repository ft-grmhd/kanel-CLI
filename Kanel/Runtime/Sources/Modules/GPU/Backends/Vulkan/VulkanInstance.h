// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_INSTANCE
#define KANEL_CLI_VULKAN_INSTANCE

#include <Modules/GPU/Backends/Vulkan/VulkanCore.h>

VkInstance kbhVulkanCreateInstance(const char** extensions_enabled, uint32_t extensions_count);
void kbhVulkanDestroyInstance(VkInstance instance);

#endif
