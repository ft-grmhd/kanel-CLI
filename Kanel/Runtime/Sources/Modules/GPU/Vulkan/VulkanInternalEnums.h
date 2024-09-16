// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_INTERNAL_ENUMS
#define KANEL_CLI_VULKAN_INTERNAL_ENUMS

typedef enum KbhVulkanQueueType
{
	KBH_VULKAN_QUEUE_COMPUTE = 0,
	KBH_VULKAN_QUEUE_TRANSFER = 1,

	KBH_VULKAN_QUEUE_END_ENUM // For internal use only
} KbhVulkanQueueType;

typedef enum KbhVulkanBufferType
{
	KBH_VULKAN_BUFFER_CONSTANT = 0,
	KBH_VULKAN_BUFFER_STAGING,
	KBH_VULKAN_BUFFER_HIGH_DYNAMIC, // Typically stored in RAM
	KBH_VULKAN_BUFFER_LOW_DYNAMIC,  // Typically stored in VRAM
} KbhVulkanBufferType;

#endif
