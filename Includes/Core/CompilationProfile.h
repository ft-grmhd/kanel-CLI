// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_COMPILATION_PROFILE
#define KANEL_CLI_CORE_COMPILATION_PROFILE

#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Try to identify the compiler
#if defined(__BORLANDC__)
	#define KANEL_CLI_COMPILER_BORDLAND
#elif defined(__clang__)
	#define KANEL_CLI_COMPILER_CLANG
	#ifdef __MINGW32__
		#define KANEL_CLI_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define KANEL_CLI_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__GNUC__) || defined(__MINGW32__)
	#define KANEL_CLI_COMPILER_GCC
	#ifdef __MINGW32__
		#define KANEL_CLI_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define KANEL_CLI_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__INTEL_COMPILER) || defined(__ICL)
	#define KANEL_CLI_COMPILER_INTEL
#elif defined(_MSC_VER)
	#define KANEL_CLI_COMPILER_MSVC
#else
	#define KANEL_CLI_COMPILER_UNKNOWN
	#warning This compiler is not fully supported
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	#define KANEL_CLI_PLAT_WINDOWS
#elif defined(__linux__)
	#define KANEL_CLI_PLAT_LINUX
	#define KANEL_CLI_PLAT_UNIX
#elif defined(__APPLE__) && defined(__MACH__)
	#define KANEL_CLI_PLAT_MACOS
#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define KANEL_CLI_PLAT_UNIX
#else
	#error Unknown environment (not Windows, not Linux, not MacOS, not Unix)
#endif

#ifndef __cplusplus // if we compile in C
	#ifdef __STDC__
		#ifdef __STDC_VERSION__
			#if __STDC_VERSION__ == 199409L
				#define KANEL_CLI_C_VERSION 1994
			#elif __STDC_VERSION__ == 199901L
				#define KANEL_CLI_C_VERSION 1999
			#elif __STDC_VERSION__ == 201112L
				#define KANEL_CLI_C_VERSION 2011
			#elif __STDC_VERSION__ == 201710L
				#define KANEL_CLI_C_VERSION 2017
			#elif __STDC_VERSION__ == 202311L
				#define KANEL_CLI_C_VERSION 2023
			#else
				#define KANEL_CLI_C_VERSION 0
			#endif
		#else
			#define KANEL_CLI_C_VERSION 0
		#endif
	#else
		#define KANEL_CLI_C_VERSION 0
	#endif
#else
	#define KANEL_CLI_C_VERSION 0
#endif

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define KANEL_CLI_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define KANEL_CLI_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
	#define KANEL_CLI_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define KANEL_CLI_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define KANEL_CLI_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define KANEL_CLI_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define KANEL_CLI_FUNC_SIG __func__
#else
	#define KANEL_CLI_FUNC_SIG "Unknown function"
#endif

#ifdef KANEL_CLI_PLAT_WINDOWS
	#ifdef KANEL_CLI_COMPILER_MSVC
		#define KANEL_CLI_EXPORT_API __declspec(dllexport)
		#define KANEL_CLI_IMPORT_API __declspec(dllimport)
	#elif defined(KANEL_CLI_COMPILER_GCC)
		#define KANEL_CLI_EXPORT_API __attribute__((dllexport))
		#define KANEL_CLI_IMPORT_API __attribute__((dllimport))
	#else
		#define KANEL_CLI_EXPORT_API
		#define KANEL_CLI_IMPORT_API
	#endif
#elif defined(KANEL_CLI_COMPILER_GCC)
	#define KANEL_CLI_EXPORT_API __attribute__((visibility("default")))
	#define KANEL_CLI_IMPORT_API __attribute__((visibility("default")))
#else
	#define KANEL_CLI_EXPORT_API
	#define KANEL_CLI_IMPORT_API
#endif

#if defined(KANEL_CLI_PLAT_WINDOWS)
	#define KANEL_CLI_LIB_EXTENSION ".dll"
#elif defined(KANEL_CLI_PLAT_UNIX)
	#define KANEL_CLI_LIB_EXTENSION ".so"
#elif defined(KANEL_CLI_PLAT_MACOS)
	#define KANEL_CLI_LIB_EXTENSION ".dylib"
#elif defined(KANEL_CLI_PLAT_WASM)
	#define KANEL_CLI_LIB_EXTENSION ".wasm"
#endif

#ifdef KANEL_CLI_COMPILER_MSVC
	#define KANEL_CLI_LIB_PREFIX ""
#else
	#define KANEL_CLI_LIB_PREFIX "lib"
#endif

#if !defined(KANEL_CLI_NO_FORCEINLINE) && !defined(KANEL_CLI_FORCEINLINE)
	#if defined(KANEL_CLI_COMPILER_CLANG) || defined(KANEL_CLI_COMPILER_GCC)
		#define KANEL_CLI_FORCEINLINE __attribute__((always_inline)) inline
	#elif defined(KANEL_CLI_COMPILER_MSVC)
		#define KANEL_CLI_FORCEINLINE __forceinline
	#else
		#define KANEL_CLI_FORCEINLINE inline
	#endif
#endif

#if !defined(KANEL_CLI_NO_WEAK) && !defined(KANEL_CLI_WEAK)
	#if defined(KANEL_CLI_COMPILER_CLANG) || defined(KANEL_CLI_COMPILER_GCC)
		#define KANEL_CLI_WEAK __attribute__((weak))
	#else
		#define KANEL_CLI_WEAK
	#endif
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__ppc64__)
	#define KANEL_CLI_64BITS
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
	#define KANEL_CLI_32BITS
#elif UINTPTR_MAX == UINT32_MAX
	#define KANEL_CLI_32BITS
#elif UINTPTR_MAX == UINT64_MAX
	#define KANEL_CLI_64BITS
#endif

#define KANEL_CLI_UNUSED_PARAM(x)
#define KANEL_CLI_UNUSED(x) (void)(x)

#if KANEL_CLI_C_VERSION >= 2023
	#if defined(KANEL_CLI_COMPILER_GCC) || defined(KANEL_CLI_COMPILER_CLANG) // for now only GCC and Clang supports nullptr
		#define KANEL_CLI_NULLPTR nullptr
	#else
		#define KANEL_CLI_NULLPTR NULL
	#endif

	#ifdef KANEL_CLI_COMPILER_GCC // for now only GCC supports constexpr
		#define KANEL_CLI_CONSTEXPR constexpr
	#else
		#define KANEL_CLI_CONSTEXPR static
	#endif
	#if defined(KANEL_CLI_COMPILER_GCC) || defined(KANEL_CLI_COMPILER_CLANG)
		#define KANEL_CLI_NONNULL(...) [[gnu::nonnull(__VA_ARGS__)]]
	#else
		#define KANEL_CLI_NONNULL(...)
	#endif
#else
	#define KANEL_CLI_NULLPTR NULL
	#define KANEL_CLI_CONSTEXPR
	#if defined(KANEL_CLI_COMPILER_GCC) || defined(KANEL_CLI_COMPILER_CLANG)
		#define KANEL_CLI_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
	#else
		#define KANEL_CLI_NONNULL(...)
	#endif
#endif

// Checking common assumptions

#if !defined(KANEL_CLI_32BITS) && !defined(KANEL_CLI_64BITS)
	#error kanel_CLI can only run on 32bit or 64bit architectures
#endif

#if KANEL_CLI_C_VERSION >= 2011 || defined(__cplusplus)
	#if KANEL_CLI_C_VERSION < 2023
		#include <assert.h>
	#endif

	static_assert(CHAR_BIT == 8, "CHAR_BIT is expected to be 8");

	static_assert(sizeof(int8_t)  == 1, "int8_t is not of the correct size" );
	static_assert(sizeof(int16_t) == 2, "int16_t is not of the correct size");
	static_assert(sizeof(int32_t) == 4, "int32_t is not of the correct size");
	static_assert(sizeof(int64_t) == 8, "int64_t is not of the correct size");

	static_assert(sizeof(uint8_t)  == 1, "uint8_t is not of the correct size" );
	static_assert(sizeof(uint16_t) == 2, "uint16_t is not of the correct size");
	static_assert(sizeof(uint32_t) == 4, "uint32_t is not of the correct size");
	static_assert(sizeof(uint64_t) == 8, "uint64_t is not of the correct size");
#else
	#define STATIC_ASSERT(COND, MSG) typedef char static_assertion___##MSG[(COND)?1:-1]

	STATIC_ASSERT(CHAR_BIT == 8, CHAR_BIT_is_expected_to_be_8);

	STATIC_ASSERT(sizeof(int8_t)  == 1,  int8_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int16_t) == 2, int16_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int32_t) == 4, int32_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int64_t) == 8, int64_t_is_not_of_the_correct_size);

	STATIC_ASSERT(sizeof(uint8_t)  == 1,  uint8_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint16_t) == 2, uint16_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint32_t) == 4, uint32_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint64_t) == 8, uint64_t_is_not_of_the_correct_size);
#endif

#endif
