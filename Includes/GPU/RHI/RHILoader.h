// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_LOADER
#define KANEL_CLI_RHI_LOADER

#include <GPU/RHI/RHIDefs.h>
#include <GPU/RHI/RHIEnums.h>

typedef KbhRHIResult(*PFN_kbhRHIBackendInitContext)(KbhRHIOpaqueHandle);
typedef void        (*PFN_kbhRHIBackendUninitContext)(KbhRHIOpaqueHandle);
typedef KbhRHIResult(*PFN_kbhRHIBackendCreateComputePipeline)(KbhRHIOpaqueHandle, KbhRHIOpaqueHandle);
typedef void        (*PFN_kbhRHIBackendDestroyComputePipeline)(KbhRHIOpaqueHandle, KbhRHIOpaqueHandle);

typedef struct KbhRHILoaderPFNs
{
	PFN_kbhRHIBackendInitContext f_kbhRHIBackendInitContext;
	PFN_kbhRHIBackendUninitContext f_kbhRHIBackendUninitContext;
	PFN_kbhRHIBackendCreateComputePipeline f_kbhRHIBackendCreateComputePipeline;
	PFN_kbhRHIBackendDestroyComputePipeline f_kbhRHIBackendDestroyComputePipeline;
} KbhRHILoaderPFNs;

#endif
