// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Application.h>
#include <Core/RuntimeOptions.h>
#include <Core/ModuleLoader.h>
#include <Core/ModulesBindPoints/GPU/GPUSupport.h>

#include <stdlib.h>

typedef struct KbhCoreApplication
{
	
} KbhCoreApplication;

static KbhCoreApplication* core_application = KANEL_CLI_NULLPTR;

int32_t kbhInitCoreApplication(int argc, char** argv)
{
	core_application = (KbhCoreApplication*)malloc(sizeof(KbhCoreApplication));
	if(!core_application)
		return -1;
	if(!kbhRuntimeOptionsParseCmd(argc, argv))
	{
		kbhRuntimeOptionsClear();
		return 0;
	}
	kbhCoreLoadAllModulesFromCmdLine();
	return 1;
}

void kbhLaunchCoreApplication()
{
	// tests
	char dummy[1024];
	if(kbhRuntimeOptionsGetString("gpu", dummy, 1024))
	{
		kbhInitGPUSupport();
		kbhUninitGPUSupport();
	}
}

void kbhShutdownCoreApplication()
{
	kbhCoreUnloadAllModules();
	kbhRuntimeOptionsClear();
	free((void*)core_application);
}
