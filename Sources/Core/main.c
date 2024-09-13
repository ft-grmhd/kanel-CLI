// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/RuntimeOptions.h>
#include <Core/ModuleLoader.h>

#include <GPU/GPUSupport.h>

int main(int argc, char *argv[])
{
	if(!kbhRuntimeOptionsParseCmd(argc, argv))
	{
		kbhRuntimeOptionsClear();
		return 0;
	}
	kbhCoreLoadAllModulesFromCmdLine();

	// tests
	char dummy[1024];
	if(kbhRuntimeOptionsGetString("gpu", dummy, 1024))
	{
		kbhInitGPUSupport();
		kbhUninitGPUSupport();
	}

	kbhCoreUnloadAllModules();
	kbhRuntimeOptionsClear();
	return 0;
}
