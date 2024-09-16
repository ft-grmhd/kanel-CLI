// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/ModuleLoader.h>
#include <Core/RuntimeOptions.h>
#include <GPU/RHIBindPoint.h>
#include <Core/Logs.h>

#include <Config.h>

#include <stdlib.h>
#include <string.h>

typedef struct KbhCoreModuleDescriptor
{
	struct KbhCoreModuleDescriptor* next;
	KbhLibModule module;
	KbhCoreModule type;
} KbhCoreModuleDescriptor;

static KbhCoreModuleDescriptor* head = KANEL_CLI_NULLPTR;

static void kbhAddLibModule(KbhLibModule module, KbhCoreModule type)
{
	KbhCoreModuleDescriptor* descriptor = (KbhCoreModuleDescriptor*)malloc(sizeof(KbhCoreModuleDescriptor));
	if(!descriptor)
		kbhFatalError("allocation failed");
	descriptor->next = head;
	descriptor->module = module;
	descriptor->type = type;
	head = descriptor;
}

static void kbhRemoveLibModule(KbhCoreModule type)
{
	KbhCoreModuleDescriptor* ptr = head;
	if(ptr == KANEL_CLI_NULLPTR)
		return;
	if(ptr->type == type)
	{
		head = ptr->next;
		free((void*)ptr);
		return;
	}
	while(ptr->next != KANEL_CLI_NULLPTR)
	{
		if(ptr->type == type)
		{
			KbhCoreModuleDescriptor* tmp = ptr->next;
			ptr->next = ptr->next->next;
			free((void*)tmp);
			return;
		}
		ptr = ptr->next;
	}
}

#define STRING_CHECK_SIZE 512

void kbhCoreLoadAllModulesFromCmdLine()
{
	char string_check[STRING_CHECK_SIZE] = { 0 };
	if(kbhRuntimeOptionsGetString("gpu", string_check, STRING_CHECK_SIZE))
		kbhCoreLoadModule(KBH_GPU_MODULE);
}

typedef KbhRHILoaderPFNs(*PFN_kbhRHILoaderPFNs)(void);

static void kbhCompleteRHILoad(KbhLibModule module)
{
	kbhVerify(module != KBH_NULL_LIB_MODULE);
	PFN_kbhRHILoaderPFNs loader_function = (PFN_kbhRHILoaderPFNs)kbhLoadSymbolFromLibModule(module, "kbhRHIFrontendAcquirePFNs");
	*kbhGetRHILoaderPFNs() = loader_function();
	if(strcmp(kbhGetRHILoaderPFNs()->f_kbhRHIBackendGetBuildVersion(), KANEL_CLI_VERSION) != 0)
		kbhFatalError("Module loader : cannot load RHI backend, conflict in build versions");
	kbhDebugLog("Module loader : "KANEL_CLI_LIB_PREFIX "kanel_gpu" KANEL_CLI_LIB_EXTENSION " loaded ");
}

KbhLibModule kbhCoreLoadModule(KbhCoreModule type)
{
	KbhLibModule module = KBH_NULL_LIB_MODULE;
	switch(type)
	{
		case KBH_GPU_MODULE: module = kbhLoadLibrary(KANEL_CLI_LIB_PREFIX "kanel_gpu" KANEL_CLI_LIB_EXTENSION); kbhCompleteRHILoad(module); break;
		default: break;
	};
	kbhVerify(module != KBH_NULL_LIB_MODULE);
	kbhAddLibModule(module, type);
	return module;
}


void kbhCoreUnloadAllModules()
{
	while(head != KANEL_CLI_NULLPTR)
		kbhCoreUnloadModule(head->type);
}

void kbhCoreUnloadModule(KbhCoreModule type)
{
	kbhRemoveLibModule(type);
	kbhDebugLog("Module loader : unloaded a module");
}
