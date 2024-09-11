// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_CORE_EVENT_BUS
#define KANEL_CLI_CORE_EVENT_BUS

#include <Core/Enums.h>

typedef void(*PFN_kbhEventListenerCallback)(KbhEvents);

KANEL_CLI_CORE_API typedef struct KbhEventListener
{
	PFN_kbhEventListenerCallback callback;
	const char* name;
} KbhEventListener;

KANEL_CLI_CORE_API void kbhEventBusSend(const char* listener_name, KbhEvents event) KANEL_CLI_NONNULL(1);
KANEL_CLI_CORE_API void kbhEventBusSendBroadcast(KbhEvents event);
KANEL_CLI_CORE_API void kbhEventBusRegisterListener(const char* name, PFN_kbhEventListenerCallback callback) KANEL_CLI_NONNULL(1, 2);

#endif
