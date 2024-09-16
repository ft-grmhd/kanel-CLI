// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_CORE
#define KANEL_CLI_RHI_CORE

#include <Modules/GPU/RHI/RHIDefs.h>
#include <Modules/GPU/RHI/RHIEnums.h>
#include <Modules/GPU/RHI/RHILoader.h>

#include <Core/Logs.h>

static const KbhRHIType KBH_RHI_DEFAULT_BACKEND = KBH_RHI_VULKAN;

KANEL_CLI_RHI_DEFINE_NULLABLE_HANDLE(KbhRHIContext);

KANEL_CLI_NONNULL(1) KbhRHIResult kbhRHIInit(KbhRHIContext* context);
KbhRHIType kbhRHIGetBackendType(KbhRHIContext context);
void kbhRHIUninit(KbhRHIContext context);

KANEL_CLI_RHI_API KbhRHILoaderPFNs kbhRHIFrontendAcquirePFNs();

static inline const char* kbhVerbaliseRHIResult(KbhRHIResult result)
{
	switch(result)
	{
		case KBH_RHI_SUCCESS: return "Success";
		case KBH_RHI_INCOMPLETE: return "Incomplete data";
		case KBH_RHI_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";

		default: return "Unknown RHI error";
	}
	return KANEL_CLI_NULLPTR; // just to avoid warnings
}

static inline void kbhCheckRHIBackend(KbhRHIResult result, const char* file, const char* function, int line)
{
	if(result < KBH_RHI_SUCCESS)
		kbhFatalErrorBackend("RHI check failed due to: %s", file, function, line, kbhVerbaliseRHIResult(result));
	else if(result > KBH_RHI_SUCCESS)
		kbhErrorBackend("RHI check failed due to: %s", file, function, line, kbhVerbaliseRHIResult(result));
}

#undef kbhCheckRHI
#define kbhCheckRHI(res) kbhCheckRHIBackend(res, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#endif
