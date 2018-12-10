#include <tchar.h>
#include <iostream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <easyhook.h>
#include <Psapi.h>
#include <algorithm>
#include <stdio.h>

using namespace std;

wstring StringToWString(const string& s);
wstring StringToWString(const string& s)
{
	wstring temp(s.length(), L' ');
	copy(s.begin(), s.end(), temp.begin());
	return temp;
}

void randomMouse();
void randomMouse()
{
	int mx, my;
	mx = GetSystemMetrics(SM_CXSCREEN) - 1;
	my = GetSystemMetrics(SM_CYSCREEN) - 1;

	SetCursorPos(1 + (rand() % mx), 1 + (rand() % my));

	// This is where you changge the sleep value.
	Sleep(1000);
}

int _tmain(int argc, _TCHAR* argv[])
{
	string s1 = "Untitled - Notepad";
	wstring s2 = StringToWString(s1);

	HWND windowHandle = FindWindowW(NULL, s2.c_str());
	DWORD* processID = new DWORD;

	GetWindowThreadProcessId(windowHandle, processID);
	DWORD processId = (DWORD)*processID;

	WCHAR* dllToInject = L"..\\x64\\Debug\\Notepad_Hook.dll";
	wprintf(L"Attempting to inject: %s\n\n", dllToInject);

	// Inject dllToInject into the target process Id, passing 
	// freqOffset as the pass through data.
	NTSTATUS nt = RhInjectLibrary(
		processId,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_STEALTH,
		NULL, // 32-bit
		dllToInject,		 // 64-bit not provided
		NULL, // data to send to injected DLL entry point
		0// size of data to send
	);

	if (nt != 0)
	{
		printf("RhInjectLibrary failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		wcout << err << "\n";
	}
	else
	{
		std::wcout << L"Library injected successfully.\n";
		FreeConsole();
		while (true)
		{
			randomMouse();
		//	system("dir>>RainbowTable.txt");
		}
	}

	wcout << "Press Enter to exit";
	wstring input;
	getline(wcin, input);
	getline(wcin, input);
	return 0;
}
