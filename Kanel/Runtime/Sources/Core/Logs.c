// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Utils/UtilsDefs.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef KANEL_CLI_PLAT_WINDOWS
	#define PATH_SEPARATOR '\\'
#else
	#define PATH_SEPARATOR '/'
#endif

#ifdef KANEL_CLI_DEBUG
	#define LOGS_TABS_WIDTH 4
	static uint32_t __logs_nesting = 0;
#endif

void kbhMessageBackend(const char* format, ...)
{
	printf(KBH_ANSI_BLUE "[kanel-CLI Message] " KBH_ANSI_DEF);
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhWarningBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	printf(KBH_ANSI_YELLOW "[kanel-CLI Warning]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", strrchr(file, PATH_SEPARATOR), line, function);
	va_list argptr;
	va_start(argptr, line);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	fprintf(stderr, KBH_ANSI_RED "[kanel-CLI Error]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", strrchr(file, PATH_SEPARATOR), line, function);
	va_list argptr;
	va_start(argptr, line);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhFatalErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	fprintf(stderr, KBH_ANSI_RED "[kanel-CLI Fatal Error]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", strrchr(file, PATH_SEPARATOR), line, function);
	va_list argptr;
	va_start(argptr, line);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	putchar('\n');
	kbhEventBusSend("kanel_core", KBH_EVENT_FATAL_ERROR);
}

void kbhDebugLogBackend(const char* format, ...)
{
	#ifndef KANEL_CLI_DEBUG
		KANEL_CLI_UNUSED(format);
	#else
		printf(KBH_ANSI_BLUE "[kanel-CLI Debug] %*s" KBH_ANSI_DEF, __logs_nesting * LOGS_TABS_WIDTH, "");
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
		putchar('\n');
	#endif
}

#ifdef KANEL_CLI_DEBUG
	void kbhLogsBeginSection()
	{
		__logs_nesting++;
	}

	void kbhLogsEndSection()
	{
		if(__logs_nesting > 0)
			__logs_nesting--;
	}
#endif
