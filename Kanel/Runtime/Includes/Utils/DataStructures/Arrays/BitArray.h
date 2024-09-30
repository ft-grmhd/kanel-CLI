// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_UTILS_DATA_STRUCTURES_ARRAYS_BIT
#define KANEL_CLI_UTILS_DATA_STRUCTURES_ARRAYS_BIT

#include <Core/CompilationProfile.h>

#ifdef KANEL_CLI_32BITS
	typedef uint32_t uintsys_t;
#else
	typedef uint64_t uintsys_t;
#endif

#define KANEL_CLI_N_SYS_BITS (CHAR_BIT * sizeof(uintsys_t))

typedef struct KbhBitsArray
{
	uintsys_t* pool;
	size_t size;
} KbhBitsArray;

KANEL_CLI_NONNULL(1) void kbhInitBitsArray(KbhBitsArray* array, size_t size);
KANEL_CLI_NONNULL(1) void kbhBitsArrayResetToValue(KbhBitsArray* array, bool value);
KANEL_CLI_NONNULL(1) void kbhBitsArraySet(KbhBitsArray* array, size_t index, bool value);
KANEL_CLI_NONNULL(1) bool kbhBitsArrayGet(const KbhBitsArray* array, size_t index);
KANEL_CLI_NONNULL(1) size_t kbhBitsArrayGetFirstBitIndexWithValue(const KbhBitsArray* array, bool value);
KANEL_CLI_NONNULL(1) void kbhDestroyBitsArray(KbhBitsArray* array);

#endif
