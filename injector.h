#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

class BKSInjector
{
public:
	void inject(DWORD processID, const char* dllPath);
	const char* getDllPath(const char* dllName);
};

