// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/LibLoader.h>
#include <Core/Logs.h>

#ifdef KANEL_CLI_PLAT_WINDOWS
	KANEL_CLI_IMPORT_API HMODULE __stdcall LoadLibraryA(LPCSTR);
	KANEL_CLI_IMPORT_API FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
	KANEL_CLI_IMPORT_API int __stdcall FreeLibrary(HMODULE);
#else
	#ifdef KANEL_CLI_PLAT_MACOS
		#include <stdlib.h>
	#endif
	#include <dlfcn.h>
#endif

KbhLibModule kbhLoadLibrary(const char* libpath)
{
	KbhLibModule module;
	#if defined(KANEL_CLI_PLAT_WINDOWS)
		module = LoadLibraryA(libpath);
		if(!module)
			kbhErrorFmt("Lib Loader: could not load %s", libpath);
	#else
		dlerror();
		module = dlopen(libpath, RTLD_NOW | RTLD_GLOBAL);
		if(!module)
			kbhErrorFmt("Lib Loader: could not load %s. %s", libpath, dlerror());
	#endif
	return (module ? module : KBH_NULL_LIB_MODULE);
}

PFN_kbhLibFunction kbhLoadSymbolFromLibModule(KbhLibModule module, const char* symbol)
{
	if(module == KBH_NULL_LIB_MODULE)
		return KANEL_CLI_NULLPTR;
	PFN_kbhLibFunction function;
	#ifdef KANEL_CLI_PLAT_WINDOWS
		function = (PFN_kbhLibFunction)GetProcAddress(module, symbol);
	#else
		void* symbol_ptr = (PFN_kbhLibFunction)dlsym(module, symbol);
		*(void**)(&function) = symbol_ptr;
	#endif
	return function;
}

void kbhUnloadLibrary(KbhLibModule module)
{
	if(module == KBH_NULL_LIB_MODULE)
		return;
	else {} // To avoid stupid Clang warning with lines below
	#ifdef KANEL_CLI_PLAT_WINDOWS
		FreeLibrary(module);
	#else
		dlclose(module);
	#endif
}
