// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_CORE
#define KANEL_CLI_RHI_CORE

#include <GPU/RHI/RHIDefs.h>
#include <GPU/RHI/RHIEnums.h>

KANEL_CLI_CONSTEXPR KbhRHIType KBH_RHI_DEFAULT_BACKEND = KBH_RHI_VULKAN;

KANEL_CLI_RHI_DEFINE_NULLABLE_HANDLE(KbhRHIContext);

KANEL_CLI_RHI_API KbhRHIResult kbhRHIInit(KbhRHIType backend, KbhRHIContext* context) KANEL_CLI_NONNULL(2);
KANEL_CLI_RHI_API KbhRhiType kbhRHIGetBackendType(KbhRHIContext context);
KANEL_CLI_RHI_API void kbhRHIUninit(KbhRHIContext context);

void kbhCheckRHI(KbhRHIResult result, const char* file, const char* function, int line);

#undef kbhCheckRHI
#define kbhCheckRHI(res) kbhCheckRHI(res, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#endif
