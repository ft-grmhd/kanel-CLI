// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Application.h>

int main(int argc, char *argv[])
{
	int32_t init_res = kbhInitCoreApplication(argc, argv);
	if(init_res < 1)
		return init_res;
	kbhLaunchCoreApplication();
	kbhShutdownCoreApplication();
	return 0;
}
