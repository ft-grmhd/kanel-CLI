// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_UTILS
#define KANEL_CLI_UTILS

#include <Core/CompilationProfile.h>

#ifndef KANEL_CLI_UTILS_STATIC
	#ifdef KANEL_CLI_UTILS_BUILD
		#define KANEL_CLI_UTILS_API KANEL_CLI_EXPORT_API
	#else
		#define KANEL_CLI_UTILS_API KANEL_CLI_IMPORT_API
	#endif
#else
	#define KANEL_CLI_UTILS_API
#endif

#define KBH_ANSI_RED "\033[1;31m"
#define KBH_ANSI_GREEN "\033[1;32m"
#define KBH_ANSI_BLUE "\033[1;34m"
#define KBH_ANSI_DEF "\033[1;0m"
#define KBH_ANSI_BLACK "\033[1;30m"
#define KBH_ANSI_YELLOW "\033[1;33m"
#define KBH_ANSI_MAGENTA "\033[1;35m"
#define KBH_ANSI_CYAN "\033[1;36m"
#define KBH_ANSI_WHITE "\033[1;37m"
#define KBH_ANSI_BG_RED "\033[1;41m"
#define KBH_ANSI_BG_GREEN "\033[1;42m"
#define KBH_ANSI_BG_BLUE "\033[1;44m"
#define KBH_ANSI_BG_DEF "\033[1;0m"
#define KBH_ANSI_BG_BLACK "\033[1;40m"
#define KBH_ANSI_BG_YELLOW "\033[1;43m"
#define KBH_ANSI_BG_MAGENTA "\033[1;45m"
#define KBH_ANSI_BG_CYAN "\033[1;46m"
#define KBH_ANSI_BG_WHITE "\033[1;47m"
#define KBH_ANSI_RESET "\033[1;0m"
#define KBH_ANSI_BOLD "\033[1;1m"
#define KBH_ANSI_UNDERLINE "\033[1;4m"
#define KBH_ANSI_INVERSE "\033[1;7m"
#define KBH_ANSI_BOLD_OFF "\033[1;21m"
#define KBH_ANSI_UNDERLINE_OFF "\033[1;24m"
#define KBH_ANSI_INVERSE_OFF "\033[1;27m"

#endif
