// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_STACK_ALLOCATOR
#define KANEL_CLI_CORE_STACK_ALLOCATOR

#include <Core/CompilationProfile.h>
#include <Core/Memory/PoolAllocator.h>

#define KANEL_CLI_DECLARE_STACK_POOL_ALLOCATOR(name, pool_size) \
	uint8_t stack_allocator_pool_##name[pool_size]; \
	KbhPoolAllocator name = { stack_allocator_pool_##name, (void*)(stack_allocator_pool_##name + pool_size), KANEL_CLI_NULLPTR, KANEL_CLI_NULLPTR, pool_size, 0 }

#define kbhStaticAllocStack(size) ((char[size]){ 0 })

#endif
