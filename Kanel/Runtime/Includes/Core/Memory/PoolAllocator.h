// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_POOL_ALLOCATOR
#define KANEL_CLI_CORE_POOL_ALLOCATOR

#include <stddef.h>
#include <stdint.h>

#include <Utils/DataStructures/Trees/RedBlack.h>

typedef struct KbhPoolAllocator
{
	const uint8_t* heap;
	const uint8_t* heap_end;
	KbhRedBlackTreeNode_uint64_t* used_flags;
	KbhRedBlackTreeNode_uint64_t* free_flags;
	size_t heap_size;
	size_t mem_used;
} KbhPoolAllocator;

KANEL_CLI_NONNULL(1, 2, 3) void kbhInitPoolAllocator(KbhPoolAllocator* allocator, const uint8_t* pool, const uint8_t* pool_end);
KANEL_CLI_NONNULL(1) bool kbhPoolAllocatorCanHold(KbhPoolAllocator* allocator, size_t size);
KANEL_CLI_NONNULL(1) void* kbhAllocInPool(KbhPoolAllocator* allocator, size_t size);
KANEL_CLI_NONNULL(1) void* kbhAllocInPoolAligned(KbhPoolAllocator* allocator, size_t size, size_t aligment);
KANEL_CLI_NONNULL(1) void* kbhCallocInPool(KbhPoolAllocator* allocator, size_t n, size_t size);
KANEL_CLI_NONNULL(1) void* kbhCallocInPoolAligned(KbhPoolAllocator* allocator, size_t n, size_t size, size_t aligment);
KANEL_CLI_NONNULL(1, 2) void kbhFreeInPool(KbhPoolAllocator* allocator, void* ptr);
size_t kbhGetPoolAllocatorFlagSize();

#endif
