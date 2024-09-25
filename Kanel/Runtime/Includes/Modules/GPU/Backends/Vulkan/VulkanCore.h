// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions ofc distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_CORE
#define KANEL_CLI_VULKAN_CORE

#include <Modules/GPU/Backends/Vulkan/VulkanDefs.h>

#include <Modules/GPU/RHI/RHILoader.h>
#include <Modules/GPU/RHI/RHIEnums.h>

#include <Core/Logs.h>

#include <vulkan/vulkan_core.h>

KANEL_CLI_VULKAN_DEFINE_NULLABLE_HANDLE(KbhVulkanContext);
KANEL_CLI_VULKAN_DEFINE_NULLABLE_HANDLE(KbhVulkanDevice);

typedef struct KbhVulkanContextHandler
{
	VkInstance instance;
	KbhVulkanDevice* devices;
	size_t devices_count;
} KbhVulkanContextHandler;

KANEL_CLI_NONNULL(1) KbhRHIResult kbhVulkanInit(KbhVulkanContext* context);
KbhRHIResult kbhVulkanLoadNewDevice(KbhVulkanContext context);
void kbhVulkanUninit(KbhVulkanContext context);

KANEL_CLI_VULKAN_API KbhRHILoaderPFNs kbhRHIVulkanBackendAcquirePFNs();

const char* kbhVerbaliseVkResult(VkResult result);

#undef kbhCheckVk
#define kbhCheckVk(op) \
	do { \
		VkResult result = op; \
		if(result < VK_SUCCESS) \
			kbhFatalErrorFmt("Vulkan check failed due to: %s", kbhVerbaliseVkResult(result)); \
		else if(result > VK_SUCCESS) \
			kbhErrorFmt("Vulkan check failed due to: %s", kbhVerbaliseVkResult(result)); \
	} while(0);

#endif
