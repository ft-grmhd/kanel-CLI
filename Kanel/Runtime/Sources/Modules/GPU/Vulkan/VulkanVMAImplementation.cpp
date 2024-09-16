// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Modules/GPU/Vulkan/VulkanDevice.h>

#include <Core/Logs.h>

#define VMA_ASSERT(expr) kbhAssert(expr)
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
