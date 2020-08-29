// LostPlanet Weapons Audit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <tlhelp32.h>

DWORD GetModuleBaseAddress(LPCWSTR lpszModuleName, DWORD pID);

int main()
{
	// attempt to find the LP game window
	HWND hGameWindow = FindWindow(NULL, _T("Lost Planet Colonies Ver 1.0.1.0 for Dx9"));
	DWORD pLostPlanetProcessID = NULL;
	HANDLE LostPlanetProcessHandle = NULL;
	BOOL foundGame = TRUE;
	DWORD gameBaseAddress = NULL;
	DWORD LostPlanetColoniesDX9_928318 = NULL;

	if (hGameWindow == NULL) {
		MessageBox(NULL, _T("Couldn't find the game!"), _T("Lost Planet Trainer DX9"), NULL);
		foundGame = FALSE;
		return 0;
	}

	if (foundGame) {
		if (GetWindowThreadProcessId(hGameWindow, &pLostPlanetProcessID) > 0) {
			//MessageBox(NULL, _T("FOUND!"), _T("Lost Planet Trainer DX9"), NULL);

			LostPlanetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pLostPlanetProcessID);

			if (LostPlanetProcessHandle != INVALID_HANDLE_VALUE && LostPlanetProcessHandle != NULL)
				gameBaseAddress = GetModuleBaseAddress(_T("LostPlanetColoniesDX9.exe"), pLostPlanetProcessID);

		}

		// Get value of: LostPlanetColoniesDX9.exe + 928318;
		ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(gameBaseAddress + 0x928318), &LostPlanetColoniesDX9_928318, 4, NULL);

		// Get the Zcoord Address for future reference
		ZcoordAddress = LostPlanetColoniesDX9_928318;
		std::vector<DWORD> pointsOffsets{ 0xBC, 0x30, 0x34 };
		for (int i = 0; i < pointsOffsets.size() - 1; i++) {
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(ZcoordAddress + pointsOffsets.at(i)), &ZcoordAddress, 4, NULL);
		}
		ZcoordAddress += pointsOffsets.at(pointsOffsets.size() - 1);	// Add the last offset, done!

	}















}


DWORD GetModuleBaseAddress(LPCWSTR lpszModuleName, DWORD pID)
	{
		DWORD dwModuleBaseAddress = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

		/*	TCHAR typedef explained:
				TCHAR --> WCHAR
				WCHAR --> wchar_t

				_tcscmp(const wchar_t*, const wchar_t*);
		*/

		if (Module32First(hSnapshot, &ModuleEntry32)) {
			do {

				if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {

					dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}

		CloseHandle(hSnapshot);

		return dwModuleBaseAddress;
	}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
