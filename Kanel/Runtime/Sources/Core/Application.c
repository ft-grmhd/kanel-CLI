// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Application.h>
#include <Core/Enums.h>
#include <Core/RuntimeOptions.h>
#include <Core/ModuleLoader.h>
#include <Core/ModulesBindPoints/GPU/GPUSupport.h>
#include <Core/EventBus.h>
#include <Core/Logs.h>
#include <Utils/UtilsDefs.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct KbhCoreApplication
{
	
} KbhCoreApplication;

static KbhCoreApplication* core_application = KANEL_CLI_NULLPTR;

static void kbhApplicationEventCallback(KbhEvents event)
{
	if(event == KBH_EVENT_FATAL_ERROR)
	{
		fprintf(stderr, KBH_ANSI_BG_RED "[Aborting]" KBH_ANSI_DEF "\n");
		abort();
	}
}

int32_t kbhInitCoreApplication(int argc, char** argv)
{
	core_application = (KbhCoreApplication*)malloc(sizeof(KbhCoreApplication));
	if(!core_application)
		return -1;
	kbhEventBusRegisterListener("kanel_core", kbhApplicationEventCallback);
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
		kbhMessage("Launching computations using GPU accelerated harware");
		kbhUninitGPUSupport();
	}
}

void kbhShutdownCoreApplication()
{
	kbhCoreUnloadAllModules();
	kbhRuntimeOptionsClear();
	kbhEventBusReleaseAllListeners();
	free((void*)core_application);
}
