// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Core/Application.h>

int main(int argc, char *argv[])
{
	KbhCoreApplication* application = kbhCreateCoreApplication(argc, argv);
	if(!application)
		return 1;
	kbhLaunchCoreApplication(application);
	kbhDestroyCoreApplication(application);
	return 0;
}
