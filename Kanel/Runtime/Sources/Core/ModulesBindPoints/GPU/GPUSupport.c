// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/ModulesBindPoints/GPU/GPUSupport.h>
#include <Core/ModulesBindPoints/GPU/RHIBindPoint.h>
#include <Modules/GPU/RHI/RHICore.h>
#include <Core/CompilationProfile.h>
#include <Core/Logs.h>

static KbhRHIContext __rhi_context;

bool kbhInitGPUSupport()
{
	kbhDebugLog("Initializing GPU accelerated hardware backend");
	KbhRHILoaderPFNs* pfns = kbhGetRHILoaderPFNs();
	if(pfns->f_kbhRHIBackendInitContext == KANEL_CLI_NULLPTR)
	{
		kbhError("GPU Support: " KANEL_CLI_LIB_PREFIX "kanel_gpu" KANEL_CLI_LIB_EXTENSION " is not loaded");
		return false;
	}
	kbhLogsBeginSection();
		kbhCheckRHI(pfns->f_kbhRHIBackendInitContext(&__rhi_context));
	kbhLogsEndSection();
	return true;
}

void kbhUninitGPUSupport()
{
	kbhDebugLog("Shutting down GPU accelerated hardware backend");
	KbhRHILoaderPFNs* pfns = kbhGetRHILoaderPFNs();
	kbhLogsBeginSection();
		pfns->f_kbhRHIBackendUninitContext(__rhi_context);
	kbhLogsEndSection();
}
