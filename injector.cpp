#include "injector.h"

const char* BKSInjector::getDllPath(const char* dllName)
{
	char* buffer = new char[MAX_PATH];
	GetFullPathNameA(dllName, MAX_PATH, buffer, NULL);
	return buffer;
}

void BKSInjector::inject(DWORD processID, const char* dllPath)
{


	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (hProcess == NULL)
	{
		std::cout << "Failed to open process" << std::endl;
		return;
	}

	LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (dllPathAddress == NULL)
	{
		std::cout << "Failed to allocate memory in process" << std::endl;
		CloseHandle(hProcess);
		return;
	}

	if (!WriteProcessMemory(hProcess, dllPathAddress, dllPath, strlen(dllPath) + 1, NULL))
	{
		std::cout << "Failed to write memory in process" << std::endl;
		VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, dllPathAddress, 0, NULL);
	if (hThread == NULL)
	{
		std::cout << "Failed to create remote thread" << std::endl;
		VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return;
	}

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
	CloseHandle(hProcess);
}