// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_GPU_SUPPORT
#define KANEL_CLI_GPU_SUPPORT

#include <stdbool.h>

bool kbhInitGPUSupport();
void kbhGPULaunchComputations();
void kbhUninitGPUSupport();

#endif
