// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GPU/RHIBindPoint.h>
#include <Core/CompilationProfile.h>
#include <Core/Logs.h>

KbhRHILoaderPFNs* kbhGetRHILoaderPFNs()
{
	static KbhRHILoaderPFNs loader = { 0 };
	return &loader;
}
