// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Utils/DataStructures/Arrays/BitArray.h>

#include <Core/Logs.h>

#include <stdlib.h>
#include <string.h>

void kbhInitBitsArray(KbhBitsArray* array, size_t size)
{
	array->size = (KANEL_CLI_N_SYS_BITS + size - 1) / KANEL_CLI_N_SYS_BITS;
	array->pool = (uintsys_t*)calloc(1, array->size);
}

void kbhBitsArrayResetToValue(KbhBitsArray* array, bool value)
{
	memset(array->pool, value ? 255 : 0, array->size);
}

void kbhBitsArraySet(KbhBitsArray* array, size_t index, bool value)
{
	const uintsys_t i = index / KANEL_CLI_N_SYS_BITS;
	if(i > array->size)
	{
		kbhError("Bits Array : index out of range");
		return;
	}
	#ifdef KANEL_CLI_64BITS
		const uintsys_t mask = 1ULL << (index % KANEL_CLI_N_SYS_BITS);
	#else
		const uintsys_t mask = 1U << (index % KANEL_CLI_N_SYS_BITS);
	#endif
	array->pool[i] = value ? array->pool[i] | mask : array->pool[i];
}

bool kbhBitsArrayGet(const KbhBitsArray* array, size_t index)
{
	const uintsys_t i = index / KANEL_CLI_N_SYS_BITS;
	if(i > array->size)
	{
		kbhError("Bits Array : index out of range");
		return false;
	}
	#ifdef KANEL_CLI_64BITS
		return array->pool[i] & (1ULL << (index % KANEL_CLI_N_SYS_BITS));
	#else
		return array->pool[i] & (1U << (index % KANEL_CLI_N_SYS_BITS));
	#endif
}

size_t kbhBitsArrayGetFirstBitIndexWithValue(const KbhBitsArray* array, bool value)
{
	for(size_t i = 0; i < array->size; i++)
	{
		if(!value && array->pool[i] != 0)
			continue;
		else if(value && array->pool[i] == 0)
			continue;
		for(size_t j = i * CHAR_BIT; j < i * CHAR_BIT + CHAR_BIT; j++)
		{
			if(kbhBitsArrayGet(array, j) == value)
				return j;
		}
	}
	return -1;
}

void kbhDestroyBitsArray(KbhBitsArray* array)
{
	free(array->pool);
	array->size = 0;
}
