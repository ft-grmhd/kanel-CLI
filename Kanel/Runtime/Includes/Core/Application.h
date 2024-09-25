// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_APPLICATION
#define KANEL_CLI_CORE_APPLICATION

#include <Core/CoreDefs.h>

typedef struct KbhCoreApplication
{
	
} KbhCoreApplication;

KbhCoreApplication* kbhCreateCoreApplication(int argc, char** argv);
KANEL_CLI_NONNULL(1) void kbhLaunchCoreApplication(const KbhCoreApplication* application);
KANEL_CLI_NONNULL(1) void kbhDestroyCoreApplication(const KbhCoreApplication* application);

#endif
