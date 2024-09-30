// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_STACK_ALLOCATOR
#define KANEL_CLI_CORE_STACK_ALLOCATOR

#include <Core/CompilationProfile.h>
#include <Core/Memory/PoolAllocator.h>
#include <Core/Memory/FixedAllocator.h>

#define KANEL_CLI_DECLARE_STACK_POOL_ALLOCATOR(name, pool_size) \
	uint8_t stack_allocator_pool_heap_##name[pool_size]; \
	KbhPoolAllocator name = { stack_allocator_pool_heap_##name, (void*)(stack_allocator_pool_heap_##name + pool_size), KANEL_CLI_NULLPTR, KANEL_CLI_NULLPTR, pool_size, 0 }

#define KANEL_CLI_DECLARE_STACK_FIXED_ALLOCATOR(name, pool_size, block_size) \
	uint8_t stack_allocator_fixed_heap_##name[pool_size]; \
	KbhFixedAllocator name; \
	kbhInitFixedAllocator(&name, stack_allocator_fixed_heap_##name, pool_size, block_size)

#define kbhStaticAllocStack(size) ((char[size]){ 0 })

#endif
