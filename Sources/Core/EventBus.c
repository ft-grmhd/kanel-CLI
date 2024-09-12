// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/EventBus.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct KbhEventListener
{
	PFN_kbhEventListenerCallback callback;
	const char* name;
} KbhEventListener;

static KbhEventListener* __kbh_listeners = KANEL_CLI_NULLPTR;
static size_t __kbh_listeners_size = 0;
static size_t __kbh_listeners_capacity = 0;

void kbhEventBusSend(const char* listener_name, KbhEvents event) KANEL_CLI_NONNULL(1)
{
	for(size_t i = 0; i < __kbh_listeners_size; i++)
	{
		if(strcmp(__kbh_listeners[i].name, listener_name) == 0)
		{
			__kbh_listeners[i].callback(event);
			return;
		}
	}
	kbhWarning("Event Bus : could not find listener '%s'", listener_name);
}

void kbhEventBusSendBroadcast(KbhEvents event)
{
	for(size_t i = 0; i < __kbh_listeners_size; i++)
		__kbh_listeners[i].callback(event);
}

void kbhEventBusRegisterListener(const char* name, PFN_kbhEventListenerCallback callback) KANEL_CLI_NONNULL(1, 2)
{
	if(__kbh_listeners_size == __kbh_listeners_capacity)
	{
		// Resize the dynamic array if necessary
		__kbh_listeners_capacity += 2;
		__kbh_listeners = (KbhEventListener*)realloc(__kbh_listeners, __kbh_listeners_capacity * sizeof(KbhEventListener));
	}

	__kbh_listeners[__kbh_listeners_size].callback = callback;
	__kbh_listeners[__kbh_listeners_size].name = strdup(name);
	__kbh_listeners_size++;
}

void kbhEventBusReleaseAllListeners()
{
	for(size_t i = 0; i < __kbh_listeners_size; i++)
		free(__kbh_listeners[i].name);
	free(__kbh_listeners);
	__kbh_listeners = KANEL_CLI_NULLPTR;
	__kbh_listeners_size = 0;
	__kbh_listeners_capacity = 0;
}
