// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_SHADER_LOADER
#define KANEL_CLI_VULKAN_SHADER_LOADER

#include <Core/CompilationProfile.h>
#include <Modules/GPU/RHI/RHIEnums.h>

#include <CNZSL/CNZSL.h>

typedef struct KbhVulkanShaderLoader
{
	nzslModule* module;
} KbhVulkanShaderLoader;

KANEL_CLI_NONNULL(1) KbhRHIResult kbhVulkanCreateShaderLoader(KbhVulkanShaderLoader** loader);
KbhRHIResult kbhVulkanLoadInternalShaders(KbhVulkanShaderLoader* loader);
void kbhVulkanDestroyShaderLoader(KbhVulkanShaderLoader* loader);

#endif
