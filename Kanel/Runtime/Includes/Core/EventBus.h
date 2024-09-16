// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_EVENT_BUS
#define KANEL_CLI_CORE_EVENT_BUS

#include <Core/CompilationProfile.h>
#include <Core/Enums.h>

typedef void(*PFN_kbhEventListenerCallback)(KbhEvents);

KANEL_CLI_NONNULL(1) void kbhEventBusSend(const char* listener_name, KbhEvents event);
void kbhEventBusSendBroadcast(KbhEvents event);
KANEL_CLI_NONNULL(1, 2) void kbhEventBusRegisterListener(const char* name, PFN_kbhEventListenerCallback callback);
void kbhEventBusReleaseAllListeners();

#endif
