// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_RHI_ENUMS
#define KANEL_CLI_RHI_ENUMS

typedef enum KbhRHIType
{
	KBH_RHI_VULKAN = 0,
	KBH_RHI_NONE,

	KBH_RHI_TYPE_END_ENUM // For internal use only
} KbhRHIType;

typedef enum KbhRHIResult
{
	KBH_RHI_SUCCESS = 0,
	KBH_RHI_INCOMPLETE = 1,
	KBH_RHI_ERROR_UNKOWN = -1,
	KBH_RHI_ERROR_INITIALIZATION_FAILED = -2,
	KBH_RHI_ERROR_INVALID_HANDLE = -3,
} KbhRHIResult;

#endif
