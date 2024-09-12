// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/RHI/RHICore.h>
#include <GPU/RHI/RHILoader.h>

#include <Core/LibLoader.h>
#include <Core/Logs.h>

#ifdef KANEL_CLI_EMBEDDED_RHI_BACKENDS
	#include <GPU/Vulkan/VulkanCore.h>
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

KbhRHIResult kbhRHIInit(KbhRHIType backend, KbhRHIContext* context)
{
	*context = (KbhRHIContext)malloc(sizeof(KbhRHIContext));
	if(!*context)
		return KBH_RHI_ERROR_INITIALIZATION_FAILED;
	else {} // To avoid stupid Clang warning with lines below

	#ifndef KANEL_CLI_EMBEDDED_RHI_BACKENDS
		const char* backends_path[(int)KBH_RHI_TYPE_END_ENUM] = {
			KANEL_CLI_LIB_PREFIX "kanelcli_vulkan" KANEL_CLI_LIB_EXTENSION,
			KANEL_CLI_NULLPTR
		};
		const char* backends_loader_names[(int)KBH_RHI_TYPE_END_ENUM] = {
			"kbhRHIVulkanBackendAcquirePFNs",
			KANEL_CLI_NULLPTR
		};
	#endif

	#ifndef KANEL_CLI_EMBEDDED_RHI_BACKENDS
		__kbh_backend_module = kbhLoadLibrary(backends_path[(int)backend]);
		if(__kbh_backend_module == KBH_NULL_LIB_MODULE)
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;

		PFN_kbhRHILoaderPFNs loader_function = (PFN_kbhRHILoaderPFNs)kbhLoadSymbolFromLibModule(__kbh_backend_module, backends_loader_names[(int)backend]);
		if(!loader_function)
		{
			kbhErrorFmt("RHI : could not load the '%s' backend", backends_path[(int)backend]);
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;
		}
		(*context)->pfns = loader_function();
		kbhDebugLogFmt("RHI : '%s' backend loaded", backends_path[(int)backend]);
	#else
		KbhRHILoaderPFNs(*)(void) backends_loader[(int)KBH_RHI_TYPE_END_ENUM] = {
			&kbhRHIVulkanBackendAcquirePFNs,
			KANEL_CLI_NULLPTR
		};

		if(backend == KBH_RHI_NONE)
			return KBH_RHI_ERROR_INITIALIZATION_FAILED;
		(*context)->pfns = backends_loader[(int)backend]();
		kbhDebugLog("RHI : backend loaded");
	#endif

	kbhCheckRHI((*context)->pfns.f_kbhRHIBackendInitContext((*context)->impl_context));
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

static const char* kbhVerbaliseRHIResult(KbhRHIResult result)
{
	switch(result)
	{
		case KBH_RHI_SUCCESS: return "Success";
		case KBH_RHI_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";

		default: return "Unknown RHI error";
	}
	return KANEL_CLI_NULLPTR; // just to avoid warnings
}

void kbhCheckRHIBackend(KbhRHIResult result, const char* file, const char* function, int line)
{
	if(result != KBH_RHI_SUCCESS)
		kbhFatalErrorBackend("RHI check failed in '%s'|%s:%d due to: %s", file, function, line, kbhVerbaliseRHIResult(result));
}
