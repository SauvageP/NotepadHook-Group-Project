#include <tchar.h>
#include <iostream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <easyhook.h>
#include <Psapi.h>
#include <algorithm>

using namespace std;

wstring StringToWString(const string& s);

wstring StringToWString(const string& s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
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
		EASYHOOK_INJECT_DEFAULT,
		NULL, // 32-bit
		dllToInject,		 // 64-bit not provided
		NULL, // data to send to injected DLL entry point
		0// size of data to send
	);

	if (nt != 0)
	{
		printf("RhInjectLibrary failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}
	else
	{
		std::wcout << L"Library injected successfully.\n";
	}

	std::wcout << "Press Enter to exit";
	std::wstring input;
	std::getline(std::wcin, input);
	std::getline(std::wcin, input);
	return 0;
}
