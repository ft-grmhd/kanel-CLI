// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_CORE
#define KANEL_CLI_RHI_CORE

#include <GPU/RHI/RHIDefs.h>
#include <GPU/RHI/RHIEnums.h>

KANEL_CLI_RHI_API typedef struct KbhRHICore
{
	OpaqueHandle impl_core;
	KbhRHIType impl_type;
} KbhRHICore;

KANEL_CLI_RHI_API [[nodiscard]] KbhRHIResult kbhRHIInit(KbhRHIType implementation_type, KbhRHICore* core) KANEL_CLI_NONNULL(2);
KANEL_CLI_RHI_API void kbhRHIUninit(KbhRHICore* core) KANEL_CLI_NONNULL(1);

#endif
