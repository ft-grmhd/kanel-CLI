// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_FIXED_ALLOCATOR
#define KANEL_CLI_CORE_FIXED_ALLOCATOR

#include <stddef.h>
#include <stdint.h>

#include <Utils/DataStructures/Arrays/BitArray.h>

typedef struct KbhFixedAllocator
{
	KbhBitsArray bits;
	const uint8_t* heap;
	size_t heap_size;
	size_t block_size;
} KbhFixedAllocator;

KANEL_CLI_NONNULL(1, 2) void kbhInitFixedAllocator(KbhFixedAllocator* allocator, const uint8_t* heap, size_t heap_size, size_t block_size);
KANEL_CLI_NONNULL(1) bool kbhFixedAllocatorCanHold(KbhFixedAllocator* allocator, size_t size);
KANEL_CLI_NONNULL(1, 2) bool kbhFixedAllocatorContains(KbhFixedAllocator* allocator, void* ptr);
KANEL_CLI_NONNULL(1) void* kbhAllocInFixed(KbhFixedAllocator* allocator, size_t size);
KANEL_CLI_NONNULL(1) void* kbhCallocInFixed(KbhFixedAllocator* allocator, size_t n, size_t size);
KANEL_CLI_NONNULL(1, 2) void kbhFreeInFixed(KbhFixedAllocator* allocator, void* ptr);

#endif
