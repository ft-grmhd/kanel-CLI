// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_LIB_LOADER
#define KANEL_CLI_CORE_LIB_LOADER

#include <Core/CoreDefs.h>

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

#ifdef KANEL_CLI_PLAT_WINDOWS
	typedef HMODULE KbhLibModule;
#else
	typedef void* KbhLibModule;
#endif

typedef void(*PFN_kbhLibFunction)(void);

static const KbhLibModule KBH_NULL_LIB_MODULE = KANEL_CLI_NULLPTR;

KANEL_CLI_NONNULL(1) KbhLibModule kbhLoadLibrary(const char* libpath); // Returns KBH_NULL_LIB_MODULE in case of failure
KANEL_CLI_NONNULL(2) PFN_kbhLibFunction kbhLoadSymbolFromLibModule(KbhLibModule module, const char* symbol);
void kbhUnloadLibrary(KbhLibModule module);

#endif
