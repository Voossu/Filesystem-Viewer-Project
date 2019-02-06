#include "MainForm.h"

#include <Windows.h>

using namespace Drive;

// The entry point to the program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);
	if (ver.dwPlatformId != VER_PLATFORM_WIN32_NT) {
		MessageBox::Show("Starting is not possible. Requires Windows NT family!", L"Error");
		return 1;
	}

	try {
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		Application::Run(gcnew MainForm);
	}
	catch (Exception ^ex) {
		MessageBox::Show(ex->Message, L"Unknown error!");
	}

	return 0;
}
