// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Backends/Vulkan/VulkanShaderLoader.h>

#include <stdint.h>
#include <stdlib.h>

// Modules
const uint8_t maths_constants_module[] = {
	#include <Modules/GPU/Backends/Vulkan/Resources/Shaders/Modules/Maths/Constants.nzslb.h>
};

KbhRHIResult kbhVulkanCreateShaderLoader(KbhVulkanShaderLoader** loader)
{
	*loader = (KbhVulkanShaderLoader*)malloc(sizeof(KbhVulkanShaderLoader));
	if(!*loader)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	(*loader)->module = nzslModuleCreate();
	return KBH_RHI_SUCCESS;
}

KbhRHIResult kbhVulkanLoadInternalShaders(KbhVulkanShaderLoader* loader)
{
	if(loader == KANEL_CLI_NULLPTR)
		return KBH_RHI_ERROR_INVALID_HANDLE;

	return KBH_RHI_SUCCESS;
}

void kbhVulkanDestroyShaderLoader(KbhVulkanShaderLoader* loader)
{
	if(loader == KANEL_CLI_NULLPTR)
		return;
	nzslModuleDestroy(loader->module);
	free(loader);
}
