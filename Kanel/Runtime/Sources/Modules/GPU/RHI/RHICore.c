// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include "Modules/GPU/RHI/RHIEnums.h"
#include <Modules/GPU/RHI/RHICore.h>
#include <Modules/GPU/RHI/RHILoader.h>

#include <Config.h>

#include <Core/LibLoader.h>
#include <Core/Logs.h>

#include <string.h>

#ifdef KANEL_CLI_EMBEDDED_RHI_BACKENDS
	#include <Modules/GPU/Backends/Vulkan/VulkanCore.h>
#endif

#include <stdlib.h>

struct KbhRHIContextHandler
{
	KbhRHILoaderPFNs pfns;
	KbhRHIOpaqueHandle impl_context;
	KbhRHIType impl_type;
};

static KbhLibModule __kbh_backend_module = KBH_NULL_LIB_MODULE;

typedef KbhRHILoaderPFNs(*PFN_kbhRHILoaderPFNs)(void);

static const char* kbhGetBuildVersion()
{
	return KANEL_CLI_VERSION;
}

KbhRHIResult kbhRHIInit(KbhRHIContext* context)
{
	*context = (KbhRHIContext)malloc(sizeof(struct KbhRHIContextHandler));
	if(!*context)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	else {} // To avoid stupid Clang warning with lines below

	#ifndef KANEL_CLI_EMBEDDED_RHI_BACKENDS
		const char* backends_path[(int)KBH_RHI_TYPE_END_ENUM] = {
			KANEL_CLI_LIB_PREFIX "kanel_vulkan" KANEL_CLI_LIB_EXTENSION,
			KANEL_CLI_NULLPTR
		};
		const char* backends_loader_names[(int)KBH_RHI_TYPE_END_ENUM] = {
			"kbhRHIVulkanBackendAcquirePFNs",
			KANEL_CLI_NULLPTR
		};
	#endif

	KbhRHIType backend = KBH_RHI_DEFAULT_BACKEND; // TODO : possibilty to override backend in the command line

	#ifndef KANEL_CLI_EMBEDDED_RHI_BACKENDS
		__kbh_backend_module = kbhLoadLibrary(backends_path[(int)backend]);
		if(__kbh_backend_module == KBH_NULL_LIB_MODULE)
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;

		PFN_kbhRHILoaderPFNs loader_function = (PFN_kbhRHILoaderPFNs)kbhLoadSymbolFromLibModule(__kbh_backend_module, backends_loader_names[(int)backend]);
		if(!loader_function)
		{
			kbhErrorFmt("RHI : could not load the %s backend", backends_path[(int)backend]);
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;
		}
		(*context)->pfns = loader_function();
		if(strcmp((*context)->pfns.f_kbhRHIBackendGetBuildVersion(), kbhGetBuildVersion()) != 0)
		{
			kbhErrorFmt("RHI : cannot load %s backend, conflict in build versions", backends_path[(int)backend]);
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;
		}
		kbhDebugLogFmt("RHI : %s backend loaded", backends_path[(int)backend]);
	#else
		PFN_kbhRHILoaderPFNs backends_loader[(int)KBH_RHI_TYPE_END_ENUM] = {
			(PFN_kbhRHILoaderPFNs)kbhRHIVulkanBackendAcquirePFNs,
			KANEL_CLI_NULLPTR
		};

		if(backend == KBH_RHI_NONE)
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;
		(*context)->pfns = backends_loader[(int)backend]();
		kbhDebugLog("RHI : backend loaded");
	#endif

	kbhCheckRHI((*context)->pfns.f_kbhRHIBackendInitContext(&(*context)->impl_context));
	(*context)->impl_type = backend;
	return KBH_RHI_SUCCESS;
}

KbhRHIType kbhRHIGetBackendType(KbhRHIContext context)
{
	if(context == KANEL_CLI_NULLPTR)
		return KBH_RHI_NONE;
	return context->impl_type;
}

void kbhRHIUninit(KbhRHIContext context)
{
	if(context == KANEL_CLI_NULLPTR)
		return;
	context->pfns.f_kbhRHIBackendUninitContext(context->impl_context);
	free(context);
	kbhUnloadLibrary(__kbh_backend_module);
	kbhDebugLog("RHI : backend unloaded");
}

KANEL_CLI_RHI_API KbhRHILoaderPFNs kbhRHIFrontendAcquirePFNs()
{
	KbhRHILoaderPFNs loader = { 0 };
	loader.f_kbhRHIBackendInitContext = (PFN_kbhRHIBackendInitContext)&kbhRHIInit;
	loader.f_kbhRHIBackendUninitContext = (PFN_kbhRHIBackendUninitContext)&kbhRHIUninit;
	loader.f_kbhRHIBackendGetBuildVersion = (PFN_kbhRHIBackendGetBuildVersion)&kbhGetBuildVersion;
	return loader;
}
