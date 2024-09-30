// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Memory/PoolAllocator.h>
#include <Core/Logs.h>

#include <string.h>

void kbhInitPoolAllocator(KbhPoolAllocator* allocator, const uint8_t* pool, const uint8_t* pool_end)
{
	allocator->heap = pool;
	allocator->heap_end = pool_end;
	allocator->used_flags = KANEL_CLI_NULLPTR;
	allocator->free_flags = KANEL_CLI_NULLPTR;
	allocator->heap_size = pool_end - pool;
	allocator->mem_used = 0;
}

bool kbhPoolAllocatorCanHold(KbhPoolAllocator* allocator, size_t size)
{
	return (size < allocator->heap_size - allocator->mem_used);
}

bool kbhPoolAllocatorContains(KbhPoolAllocator* allocator, void* ptr)
{
	return (const uint8_t*)ptr >= allocator->heap && (const uint8_t*)ptr < allocator->heap_end;
}

void* kbhCallocInPool(KbhPoolAllocator* allocator, size_t n, size_t size)
{
	return kbhCallocInPoolAligned(allocator, n, size, 0);
}

void* kbhCallocInPoolAligned(KbhPoolAllocator* allocator, size_t n, size_t size, size_t aligment)
{
	void* ptr = kbhAllocInPoolAligned(allocator, size * n, aligment);
	memset(ptr, 0, size * n);
	return ptr;
}

void* kbhAllocInPool(KbhPoolAllocator* allocator, size_t size)
{
	return kbhAllocInPoolAligned(allocator, size, 0);
}

void* kbhAllocInPoolAligned(KbhPoolAllocator* allocator, size_t size, size_t aligment)
{
	kbhVerify(allocator->heap);
	kbhVerify(allocator->heap_end);
	if(!kbhPoolAllocatorCanHold(allocator, sizeof(KbhRedBlackTreeNode_uint64_t) + aligment + size))
		return KANEL_CLI_NULLPTR;

	void* ptr;

	if(allocator->free_flags != KANEL_CLI_NULLPTR)
	{
		KbhRedBlackTreeNode_uint64_t* node = kbhRedBlackTreeFind_uint64_t(allocator->free_flags, size);
		if(node != KANEL_CLI_NULLPTR)
		{
			ptr = (void*)((uint8_t*)node + sizeof(KbhRedBlackTreeNode_uint64_t) + aligment);
			allocator->free_flags = kbhRedBlackTreeBSTRemoveNode_uint64_t(allocator->free_flags, node, KANEL_CLI_NULLPTR);
			allocator->used_flags = kbhRedBlackTreeBSTInsert_uint64_t(allocator->used_flags, node);
			KbhRedBlackTreeFixup_uint64_t(allocator->used_flags, node);
			allocator->used_flags->is_red = false;
			return ptr;
		}
	}

	KbhRedBlackTreeNode_uint64_t* new_node = (KbhRedBlackTreeNode_uint64_t*)(allocator->heap + allocator->mem_used);
	new_node->parent = KANEL_CLI_NULLPTR;
	new_node->right = KANEL_CLI_NULLPTR;
	new_node->left = KANEL_CLI_NULLPTR;
	new_node->data = size;
	new_node->is_red = true;

	allocator->used_flags = kbhRedBlackTreeBSTInsert_uint64_t(allocator->used_flags, new_node);
	KbhRedBlackTreeFixup_uint64_t(allocator->used_flags, new_node);
	allocator->used_flags->is_red = false;

	ptr = (void*)(allocator->heap + allocator->mem_used + sizeof(KbhRedBlackTreeNode_uint64_t) + aligment);

	allocator->mem_used += sizeof(KbhRedBlackTreeNode_uint64_t) + aligment + size;
	return ptr;
}

void kbhFreeInPool(KbhPoolAllocator* allocator, void* ptr)
{
	kbhVerify(allocator->heap);
	if(!kbhPoolAllocatorContains(allocator, ptr))
	{
		kbhError("Pool Allocator: trying to free a pointer allocated by another allocator");
		return;
	}
	KbhRedBlackTreeNode_uint64_t* node = kbhRedBlackTreeFindNodePtr_uint64_t(allocator->used_flags, (KbhRedBlackTreeNode_uint64_t*)(ptr - sizeof(KbhRedBlackTreeNode_uint64_t)));
	if(node == KANEL_CLI_NULLPTR)
		kbhWarning("Pool allocator: could not find the flag of an allocation");

	allocator->used_flags = kbhRedBlackTreeBSTRemoveNode_uint64_t(allocator->used_flags, node, KANEL_CLI_NULLPTR);
	allocator->free_flags = kbhRedBlackTreeBSTInsert_uint64_t(allocator->free_flags, node);
	KbhRedBlackTreeFixup_uint64_t(allocator->free_flags, node);
}

size_t kbhGetPoolAllocatorFlagSize()
{
	return sizeof(KbhRedBlackTreeNode_uint64_t) * 2; // 2 trees
}
