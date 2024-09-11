// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_ENUMS
#define KANEL_CLI_RHI_ENUMS

#include <GPU/RHI/RHIDefs.h>

KANEL_CLI_RHI_API typedef enum KbhRHIType
{
	KBH_RHI_VULKAN = 0,
} KbhRHIType;

KANEL_CLI_RHI_API typedef enum KbhRHIResult
{
	KBH_RHI_SUCCESS = 0,
	KBH_RHI_ERROR_UNKOWN = -1,
	KBH_RHI_ERROR_INITIALIZATION_FAILED = -2,
} KbhRHIResult;

#endif
