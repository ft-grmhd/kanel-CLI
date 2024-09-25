// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Application.h>
#include <Core/RuntimeOptions.h>
#include <Core/ModuleLoader.h>
#include <GPU/GPUSupport.h>

#include <stdlib.h>

KbhCoreApplication* kbhCreateCoreApplication(int argc, char** argv)
{
	KbhCoreApplication* application = (KbhCoreApplication*)malloc(sizeof(KbhCoreApplication));
	if(!application)
		return KANEL_CLI_NULLPTR;
	if(!kbhRuntimeOptionsParseCmd(argc, argv))
	{
		kbhRuntimeOptionsClear();
		return 0;
	}
	kbhCoreLoadAllModulesFromCmdLine();
	return application;
}

void kbhLaunchCoreApplication(const KbhCoreApplication* application)
{
	// tests
	char dummy[1024];
	if(kbhRuntimeOptionsGetString("gpu", dummy, 1024))
	{
		kbhInitGPUSupport();
		kbhUninitGPUSupport();
	}
}

void kbhDestroyCoreApplication(const KbhCoreApplication* application)
{
	kbhCoreUnloadAllModules();
	kbhRuntimeOptionsClear();
	free((void*)application);
}
