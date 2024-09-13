// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_RUNTIME_OPTIONS
#define KANEL_CLI_CORE_RUNTIME_OPTIONS

#include <Core/CompilationProfile.h>

#include <stdint.h>
#include <stddef.h>

KANEL_CLI_NONNULL(2) bool kbhRuntimeOptionsParseCmd(int argc, char** argv); // Returns false when the simulation should not start
KANEL_CLI_NONNULL(1, 2) bool kbhRuntimeOptionsGetString(const char* opt_name, char* dst, size_t dst_len);
KANEL_CLI_NONNULL(1, 2) bool kbhRuntimeOptionsGetFloat64(const char* opt_name, double* dst);
KANEL_CLI_NONNULL(1, 2) bool kbhRuntimeOptionsGetFloat32(const char* opt_name, float* dst);
KANEL_CLI_NONNULL(1, 2) bool kbhRuntimeOptionsGetInt(const char* opt_name, int32_t* dst);
KANEL_CLI_NONNULL(1, 2) bool kbhRuntimeOptionsGetBool(const char* opt_name, bool* dst);
void kbhRuntimeOptionsClear();

#endif
