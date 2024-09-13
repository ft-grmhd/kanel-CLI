// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_COMPUTE
#define KANEL_CLI_RHI_COMPUTE

#include <Modules/GPU/RHI/RHIDefs.h>
#include <Modules/GPU/RHI/RHIEnums.h>
#include <Modules/GPU/RHI/RHICore.h>

KANEL_CLI_RHI_DEFINE_NULLABLE_HANDLE(KbhRHIComputePipeline);

KANEL_CLI_RHI_API KbhRHIResult kbhRHICreateComputePipeline(KbhRHIContext context, KbhRHIComputePipeline* pipeline);
KANEL_CLI_RHI_API void kbhRHIDestroyComputePipeline(KbhRHIContext context, KbhRHIComputePipeline pipeline);

#endif
