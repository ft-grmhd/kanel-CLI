// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE
#define KANEL_CLI_CORE

#include <Core/CompilationProfile.h>

#ifndef KANEL_CLI_CORE_STATIC
	#ifdef KANEL_CLI_CORE_BUILD
		#define KANEL_CLI_CORE_API KANEL_CLI_EXPORT_API
	#else
		#define KANEL_CLI_CORE_API KANEL_CLI_IMPORT_API
	#endif
#else
	#define KANEL_CLI_CORE_API
#endif

#endif
