#include "injector.h"

int main()
{
	BKSInjector injector;
	HWND window = FindWindowA(NULL, "Pixel Gun 3D");
	DWORD processID;
	GetWindowThreadProcessId(window, &processID);

	injector.inject(processID, injector.getDllPath("./minhook.x64.dll"));
	injector.inject(processID, injector.getDllPath("./PixelGunCheat.dll"));

	return 0;
}