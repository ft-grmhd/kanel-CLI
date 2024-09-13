// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_MODULE_LOADER
#define KANEL_CLI_CORE_MODULE_LOADER

#include <Core/LibLoader.h>

typedef enum KbhCoreModule
{
	KBH_GPU_MODULE,
} KbhCoreModule;

void kbhCoreLoadAllModulesFromCmdLine();
KbhLibModule kbhCoreLoadModule(KbhCoreModule type);
void kbhCoreUnloadAllModules();
void kbhCoreUnloadModule(KbhCoreModule type);

#endif
