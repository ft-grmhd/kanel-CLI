// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_VULKAN
#define KANEL_CLI_VULKAN

#include <Core/CompilationProfile.h>

#ifndef KANEL_CLI_VULKAN_STATIC
	#ifdef KANEL_CLI_VULKAN_BUILD
		#define KANEL_CLI_VULKAN_API KANEL_CLI_EXPORT_API
	#else
		#define KANEL_CLI_VULKAN_API KANEL_CLI_IMPORT_API
	#endif
#else
	#define KANEL_CLI_VULKAN_API
#endif

#define KANEL_CLI_VULKAN_DEFINE_NULLABLE_HANDLE(object) typedef struct object##Handler* object

#define KANEL_CLI_VULKAN_NULL_HANDLE KANEL_CLI_NULLPTR

#endif
