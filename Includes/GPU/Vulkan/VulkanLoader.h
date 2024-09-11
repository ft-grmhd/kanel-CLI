// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN_LOADER
#define KANEL_CLI_VULKAN_LOADER

#include <GPU/Vulkan/VulkanDefs.h>
#include <GPU/Vulkan/VulkanPrototypes.h>

#ifdef KANEL_CLI_PLAT_WINDOWS
	typedef const char* LPCSTR;
	typedef struct HINSTANCE__* HINSTANCE;
	typedef HINSTANCE HMODULE;
	#if defined(_MINWINDEF_)
		/* minwindef.h defines FARPROC, and attempting to redefine it may conflict with -Wstrict-prototypes */
	#elif defined(_WIN64)
		typedef __int64 (__stdcall* FARPROC)(void);
	#else
		typedef int (__stdcall* FARPROC)(void);
	#endif
#else
	#include <dlfcn.h>
#endif

#include <kvf.h>

VkResult kbhVulkanLoaderInit();
void kbhLoadInstance(VkInstance instance);
void kbhVulkanLoaderUninit();

#endif
