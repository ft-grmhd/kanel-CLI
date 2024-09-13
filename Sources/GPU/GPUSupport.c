// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/GPUSupport.h>
#include <GPU/RHIBindPoint.h>
#include <Modules/GPU/RHI/RHICore.h>
#include <Core/CompilationProfile.h>
#include <Core/Logs.h>

static KbhRHIContext __rhi_context;

bool kbhInitGPUSupport()
{
	KbhRHILoaderPFNs* pfns = kbhGetRHILoaderPFNs();
	if(pfns->f_kbhRHIBackendInitContext == KANEL_CLI_NULLPTR)
	{
		kbhError("GPU Support : " KANEL_CLI_LIB_PREFIX "kanel_gpu" KANEL_CLI_LIB_EXTENSION " is not loaded");
		return false;
	}
	kbhCheckRHI(pfns->f_kbhRHIBackendInitContext(&__rhi_context));
	return true;
}

void kbhUninitGPUSupport()
{
	KbhRHILoaderPFNs* pfns = kbhGetRHILoaderPFNs();
	pfns->f_kbhRHIBackendUninitContext(__rhi_context);
}
