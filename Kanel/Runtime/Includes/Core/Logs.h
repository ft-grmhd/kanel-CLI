// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_LOGS
#define KANEL_CLI_CORE_LOGS

#include <Core/CompilationProfile.h>

#include <stdint.h>

KANEL_CLI_NONNULL(1) void kbhMessageBackend(const char* format, ...);
KANEL_CLI_NONNULL(1, 2, 3) void kbhWarningBackend(const char* format, const char* file, const char* function, uint32_t line, ...);
KANEL_CLI_NONNULL(1, 2, 3) void kbhErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...);
KANEL_CLI_NONNULL(1, 2, 3) void kbhFatalErrorBackend(const char* format, const char* file, const char* function, uint32_t line, ...);
KANEL_CLI_NONNULL(1) void kbhDebugLogBackend(const char* format, ...);

#ifndef KANEL_CLI_DEBUG
	#define kbhLogsBeginSection();
	#define kbhLogsEndSection() ((void)(0))
#else
	void kbhLogsBeginSection();
	void kbhLogsEndSection();
#endif

#undef  kbhMessage
#define kbhMessage(format) kbhMessageBackend(format)

#undef  kbhWarning
#define kbhWarning(format) kbhWarningBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#undef  kbhError
#define kbhError(format) kbhErrorBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#undef  kbhFatalError
#define kbhFatalError(format) kbhFatalErrorBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__)

#undef  kbhDebugLog
#define kbhDebugLog(format) kbhDebugLogBackend(format)

#undef  kbhMessageFmt
#define kbhMessageFmt(format, ...) kbhMessageBackend(format, __VA_ARGS__)

#undef  kbhWarningFmt
#define kbhWarningFmt(format, ...) kbhWarningBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhErrorFmt
#define kbhErrorFmt(format, ...) kbhErrorBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhFatalErrorFmt
#define kbhFatalErrorFmt(format, ...) kbhFatalErrorBackend(format, __FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhDebugLogFmt
#define kbhDebugLogFmt(format, ...) kbhDebugLogBackend(format, __VA_ARGS__)

#ifdef KANEL_CLI_DEBUG
	#define kbhAssert(cond) \
		do { \
			if(!(cond)) \
				kbhFatalError("Assertion triggered ! '" #cond "'"); \
		} while(0)
#else
	#define kbhAssert(cond, ...) ((void)0)
#endif

#define kbhVerify(cond) \
	do { \
		if(!(cond)) \
			kbhFatalError("Verification failed ! '" #cond "'"); \
	} while(0)

#endif
