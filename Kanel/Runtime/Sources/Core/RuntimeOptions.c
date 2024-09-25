// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/RuntimeOptions.h>
#include <Core/Logs.h>

#include <stdlib.h>
#include <string.h>

#define VRGCLI
#include <vrg.h>

typedef enum KbhRuntimeOptionType
{
	KBH_RUNTIME_OPTION_FLOAT32,
	KBH_RUNTIME_OPTION_FLOAT64,
	KBH_RUNTIME_OPTION_STRING,
	KBH_RUNTIME_OPTION_BOOL,
	KBH_RUNTIME_OPTION_INT,
} KbhRuntimeOptionType;

typedef struct KbhRuntimeOption
{
	struct KbhRuntimeOption* next;
	const char* name;
	union
	{
		const char* string;
		int32_t integer;
		bool boolean;
		double f64;
		float f32;
	};
	KbhRuntimeOptionType type;
} KbhRuntimeOption;

static KbhRuntimeOption* runtime_options_head = KANEL_CLI_NULLPTR;

static void kbhAddRuntimeOptionStringToList(const char* name, const char* string)
{
	KbhRuntimeOption* option = (KbhRuntimeOption*)malloc(sizeof(KbhRuntimeOption));
	if(!option)
		kbhFatalError("allocation failed");
	option->next = runtime_options_head;
	option->name = strdup(name);
	if(!option->name)
		kbhFatalError("allocation failed");
	option->string = strdup(string);
	if(!option->string)
		kbhFatalError("allocation failed");
	option->type = KBH_RUNTIME_OPTION_STRING;
	runtime_options_head = option;
}

static void kbhAddRuntimeOptionFloat32ToList(const char* name, float f32)
{
	KbhRuntimeOption* option = (KbhRuntimeOption*)malloc(sizeof(KbhRuntimeOption));
	if(!option)
		kbhFatalError("allocation failed");
	option->next = runtime_options_head;
	option->name = strdup(name);
	if(!option->name)
		kbhFatalError("allocation failed");
	option->f32 = f32;
	option->type = KBH_RUNTIME_OPTION_FLOAT32;
	runtime_options_head = option;
}

static void kbhAddRuntimeOptionFloat64ToList(const char* name, double f64)
{
	KbhRuntimeOption* option = (KbhRuntimeOption*)malloc(sizeof(KbhRuntimeOption));
	if(!option)
		kbhFatalError("allocation failed");
	option->next = runtime_options_head;
	option->name = strdup(name);
	if(!option->name)
		kbhFatalError("allocation failed");
	option->f64 = f64;
	option->type = KBH_RUNTIME_OPTION_INT;
	runtime_options_head = option;
}

static void kbhAddRuntimeOptionIntegerToList(const char* name, int32_t integer)
{
	KbhRuntimeOption* option = (KbhRuntimeOption*)malloc(sizeof(KbhRuntimeOption));
	if(!option)
		kbhFatalError("allocation failed");
	option->next = runtime_options_head;
	option->name = strdup(name);
	if(!option->name)
		kbhFatalError("allocation failed");
	option->integer = integer;
	option->type = KBH_RUNTIME_OPTION_INT;
	runtime_options_head = option;
}

static void kbhAddRuntimeOptionBooleanToList(const char* name, bool boolean)
{
	KbhRuntimeOption* option = (KbhRuntimeOption*)malloc(sizeof(KbhRuntimeOption));
	if(!option)
		kbhFatalError("allocation failed");
	option->next = runtime_options_head;
	option->name = strdup(name);
	if(!option->name)
		kbhFatalError("allocation failed");
	option->boolean = boolean;
	option->type = KBH_RUNTIME_OPTION_BOOL;
	runtime_options_head = option;
}

bool kbhRuntimeOptionsParseCmd(int argc, char** argv)
{
	// TODO : add more options
	vrgcli("\nkanel CLI (C) 2024 kanel devs")
	{
		vrgarg("-h, --help\tDisplay this help message")
		{
			vrghelp();
			return false;
		}
		vrgarg("-v, --version\tDisplay version")
		{
			printf("kanel CLI version 0.1beta\n");
			return false;
		}
		vrgarg("--gpu [BACKEND]\tEnables GPU accelerated calculations")
		{
			kbhAddRuntimeOptionStringToList("gpu", vrgarg);
		}
		vrgarg()
		{
			kbhRuntimeOptionsClear();
			vrgusage("Unexpected argument: '%s'\n", vrgarg);
			return false;
		}
	}
	return true;
}

bool kbhRuntimeOptionsGetString(const char* opt_name, char* dst, size_t dst_len)
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		if(strcmp(ptr->name, opt_name) == 0)
		{
			if(ptr->type == KBH_RUNTIME_OPTION_STRING)
			{
				if(strlen(ptr->string) >= dst_len)
				{
					kbhErrorFmt("too small destination buffer (%u < %u)", dst_len, strlen(ptr->string) + 1);
					return false;
				}
				strncpy(dst, ptr->string, dst_len);
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}

bool kbhRuntimeOptionsGetFloat64(const char* opt_name, double* dst)
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		if(strcmp(ptr->name, opt_name) == 0)
		{
			if(ptr->type == KBH_RUNTIME_OPTION_FLOAT64)
			{
				*dst = ptr->f64;
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}

bool kbhRuntimeOptionsGetFloat32(const char* opt_name, float* dst)
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		if(strcmp(ptr->name, opt_name) == 0)
		{
			if(ptr->type == KBH_RUNTIME_OPTION_FLOAT32)
			{
				*dst = ptr->f32;
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}

bool kbhRuntimeOptionsGetInt(const char* opt_name, int32_t* dst)
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		if(strcmp(ptr->name, opt_name) == 0)
		{
			if(ptr->type == KBH_RUNTIME_OPTION_INT)
			{
				*dst = ptr->integer;
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}

bool kbhRuntimeOptionsGetBool(const char* opt_name, bool* dst)
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		if(strcmp(ptr->name, opt_name) == 0)
		{
			if(ptr->type == KBH_RUNTIME_OPTION_BOOL)
			{
				*dst = ptr->boolean;
				return true;
			}
		}
		ptr = ptr->next;
	}
	return false;
}

void kbhRuntimeOptionsClear()
{
	KbhRuntimeOption* ptr = runtime_options_head;
	while(ptr != KANEL_CLI_NULLPTR)
	{
		KbhRuntimeOption* tmp = ptr->next;
		free((void*)ptr->name);
		if(ptr->type == KBH_RUNTIME_OPTION_STRING)
			free((void*)ptr->string);
		free(ptr);
		ptr = tmp;
	}
}
