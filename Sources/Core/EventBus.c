// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/EventBus.h>

KANEL_CLI_CORE_API void kbhEventBusSend(const char* listener_name, KbhEvents event) KANEL_CLI_NONNULL(1)
{
}

KANEL_CLI_CORE_API void kbhEventBusSendBroadcast(KbhEvents event)
{
}

KANEL_CLI_CORE_API void kbhEventBusRegisterListener(const char* name, PFN_kbhEventListenerCallback callback) KANEL_CLI_NONNULL(1, 2)
{
}
