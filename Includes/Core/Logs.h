// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_LOGS
#define KANEL_CLI_CORE_LOGS

#include <Core/CompilationProfile.h>

#include <stdint.h>

KANEL_CLI_CORE_API void kbhMessage(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3);
KANEL_CLI_CORE_API void kbhWarning(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3);
KANEL_CLI_CORE_API void kbhError(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3);
KANEL_CLI_CORE_API void kbhFatalError(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3);
KANEL_CLI_CORE_API void kbhDebugLog(const char* format, const char* file, const char* function, uint32_t line, ...) KANEL_CLI_NONNULL(1, 2, 3);

#ifdef KANEL_CLI_DEBUG
	#define kbhAssert(cond) \
		do { \
			if(!cond)
				kbhFatalError("Assertion triggered !", __FILE__, KANEL_CLI_FUNC_SIG, __LINE__); \
		} while(0);
#else
	#define kbhAssert(cond) ((void)0);
#endif

#define kbhVerify(cond) \
	do { \
		if(!cond)
			kbhFatalError("Verification failed !", __FILE__, KANEL_CLI_FUNC_SIG, __LINE__); \
	} while(0);

#undef  kbhMessage
#define kbhMessage(...) kbhMessage(__FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhWarning
#define kbhWarning(...) kbhWarning(__FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhError
#define kbhError(...) kbhError(__FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhFatalError
#define kbhFatalError(...) kbhFatalError(__FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#undef  kbhDebugLog
#define kbhDebugLog(...) kbhDebugLog(__FILE__, KANEL_CLI_FUNC_SIG, __LINE__, __VA_ARGS__)

#endif
