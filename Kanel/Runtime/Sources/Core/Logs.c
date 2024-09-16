// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Utils/UtilsDefs.h>

#include <stdio.h>
#include <stdarg.h>

void kbhMessageBackend(const char* format, ...)
{
	printf(KBH_ANSI_BLUE "[kanel-CLI Message] " KBH_ANSI_DEF);
	va_list argptr;
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhWarningBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	printf(KBH_ANSI_YELLOW "[kanel-CLI Warning]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, line);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	fprintf(stderr, KBH_ANSI_RED "[kanel-CLI Error]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, line);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhFatalErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...)
{
	fprintf(stderr, KBH_ANSI_RED "[kanel-CLI Fatal Error]" KBH_ANSI_DEF " {in '%s;', line %u, '%s'} | ", file, line, function);
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
		printf(KBH_ANSI_BLUE "[kanel-CLI Debug] " KBH_ANSI_DEF);
		va_list argptr;
		va_start(argptr, line);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
		putchar('\n');
	#endif
}
