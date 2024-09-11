// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Logs.h>
#include <Utils/UtilsDefs.h>

#include <stdio.h>
#include <stdarg.h>

void kbhMessage(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3)
{
	printf(KBH_ANSI_BLUE "[kanel-CLI Message]" KBH_ANSI_DEF " {in '%s;', line %ld, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, fline);
	vfprintf(stdout, line, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhWarning(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3)
{
	printf(KBH_ANSI_MAGENTA "[kanel-CLI Warning]" KBH_ANSI_DEF " {in '%s;', line %ld, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, fline);
	vfprintf(stdout, line, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhError(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3)
{
	printf(KBH_ANSI_RED "[kanel-CLI Error]" KBH_ANSI_DEF " {in '%s;', line %ld, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, fline);
	vfprintf(stdout, line, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhFatalError(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3)
{
	printf(KBH_ANSI_RED "[kanel-CLI Fatal Error]" KBH_ANSI_DEF " {in '%s;', line %ld, '%s'} | ", file, line, function);
	va_list argptr;
	va_start(argptr, fline);
	vfprintf(stdout, line, argptr);
	va_end(argptr);
	putchar('\n');
}

void kbhDebugLog(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3)
{
	#ifdef KANEL_CLI_DEBUG
		KANEL_CLI_UNUSED(format);
		KANEL_CLI_UNUSED(file);
		KANEL_CLI_UNUSED(function);
		KANEL_CLI_UNUSED(line);
		return;
	#else
		printf(KBH_ANSI_BLUE "[kanel-CLI Message]" KBH_ANSI_DEF " {in '%s;', line %ld, '%s'} | ", file, line, function);
		va_list argptr;
		va_start(argptr, fline);
		vfprintf(stdout, line, argptr);
		va_end(argptr);
		putchar('\n');
	#endif
}
