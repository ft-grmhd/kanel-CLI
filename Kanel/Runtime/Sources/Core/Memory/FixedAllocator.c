// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include "Utils/DataStructures/Arrays/BitArray.h"
#include <Core/Memory/FixedAllocator.h>
#include <Core/Logs.h>

#include <string.h>

void kbhInitFixedAllocator(KbhFixedAllocator* allocator, const uint8_t* heap, size_t heap_size, size_t block_size)
{
	kbhVerify(heap_size % block_size == 0 && "block_size must be a multiple of the pool size");
	kbhInitBitsArray(&allocator->bits, heap_size / block_size);
	kbhBitsArrayResetToValue(&allocator->bits, true);
	allocator->heap = heap;
	allocator->heap_size = heap_size;
	allocator->block_size = block_size;
}

bool kbhFixedAllocatorCanHold(KbhFixedAllocator* allocator, size_t size)
{
	return kbhBitsArrayGetFirstBitIndexWithValue(&allocator->bits, true) != (size_t)-1;
}

bool kbhFixedAllocatorContains(KbhFixedAllocator* allocator, void* ptr)
{
	const uint8_t* heap_end = allocator->heap + allocator->heap_size;
	return (const uint8_t*)ptr >= allocator->heap && (const uint8_t*)ptr < heap_end;
}

void* kbhCallocInFixed(KbhFixedAllocator* allocator, size_t n, size_t size)
{
	kbhVerify(allocator->heap);
	if(size % allocator->block_size != 0)
	{
		kbhError("Fixed Allocator: size of a requested callocation is not a multiple of the allocator's block size");
		return KANEL_CLI_NULLPTR;
	}
	if(!kbhFixedAllocatorCanHold(allocator, n * size))
		return KANEL_CLI_NULLPTR;
	const size_t first_slot_index = kbhBitsArrayGetFirstBitIndexWithValue(&allocator->bits, true);

	size_t free_block_row = 0;
	size_t first_row_index = -1;
	for(size_t i = 0; i < allocator->bits.size; i++)
	{
		if(allocator->bits.pool[i] == 0)
		{
			free_block_row = 0;
			first_row_index = -1;
			continue;
		}
		for(size_t j = i * CHAR_BIT; j < i * CHAR_BIT + CHAR_BIT; j++)
		{
			if(kbhBitsArrayGet(&allocator->bits, j))
			{
				free_block_row++;
				if(free_block_row == n)
					goto offloop;
				if(first_row_index == (size_t)-1)
					first_row_index = j;
			}
			else
			{
				free_block_row = 0;
				first_row_index = -1;
			}
		}
	}

offloop:
	if(free_block_row != n)
	{
		kbhError("Fixed Allocator: could not find enought free consecutive blocks for a callocation");
		return KANEL_CLI_NULLPTR;
	}
	for(size_t i = first_row_index; i < first_row_index + n; i++)
		kbhBitsArraySet(&allocator->bits, i, false);

	void* ptr = (void*)(allocator->heap + allocator->block_size * first_row_index);
	memset(ptr, 0, size * n);
	return ptr;
}

void* kbhAllocInFixed(KbhFixedAllocator* allocator, size_t size)
{
	kbhVerify(allocator->heap);
	if(size > allocator->block_size)
	{
		kbhError("Fixed Allocator: size of a requested allocation is greater than the allocator's block size");
		return KANEL_CLI_NULLPTR;
	}
	if(!kbhFixedAllocatorCanHold(allocator, size))
		return KANEL_CLI_NULLPTR;
	const size_t first_slot_index = kbhBitsArrayGetFirstBitIndexWithValue(&allocator->bits, true);
	kbhBitsArraySet(&allocator->bits, first_slot_index, false);
	return (void*)(allocator->heap + allocator->block_size * first_slot_index);
}

void kbhFreeInFixed(KbhFixedAllocator* allocator, void* ptr)
{
	if(!kbhFixedAllocatorContains(allocator, ptr))
	{
		kbhError("Fixel Allocator: trying to free a pointer allocated by another allocator");
		return;
	}
	const size_t index = ((const uint8_t*)ptr - allocator->heap) / allocator->block_size;
	kbhBitsArraySet(&allocator->bits, index, true);
}
