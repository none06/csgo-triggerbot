// trigger c++.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <TlHelp32.h>
#include "Addresses.h"
#include <string>


using namespace std;
DWORD getPID(LPCWSTR windowName) {

	HWND windowHandle = FindWindowW(NULL, windowName);
	DWORD* processID = new DWORD;

	if (!GetWindowThreadProcessId(windowHandle, processID)) {
		wcout << "Impossible de trouver: " << windowName << endl;
	}
	else {
		wcout << windowName << " trouve." << endl;;
		return *processID;
	}


}

DWORD Module(LPCWSTR ModuleName, LPCWSTR windowName) {

	DWORD dwPID = getPID(windowName);
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do
		if (!lstrcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));

	cout << "Impossible de se ratacher a: Client.dll ." << endl;
	return 0;
}

int main()
{
	int bClient = Module(L"client.dll", L"Counter-Strike: Global Offensive");

	HANDLE hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, getPID(L"Counter-Strike: Global Offensive"));

	ShowWindow(FindWindow(L"ConsoleWindowClass", NULL), true);
	HWND TargetWnd = FindWindow(0, L"Counter-Strike: Global Offensive");
	HDC HDC_Desktop = GetDC(TargetWnd);

	int address = bClient + oLocalPlayer;
	ReadProcessMemory(hProcHandle, (int*)address, &LocalPlayer, sizeof(LocalPlayer), NULL);

	system("cls");
	cout << "----------None's TriggerBot---------" << endl;
	cout << "Trigger : " << "[Off]" << endl;

	do {

		if (GetAsyncKeyState(VK_F1) && toggle == 0) {
			Beep(900, 100);
			Beep(900, 100);
			toggle = 1;
			state = "[On]";
			system("cls");
			cout << "----------None's TriggerBot---------" << endl;
			cout << "Trigger : " << state << endl;
			
			
		}
		if (GetAsyncKeyState(VK_F1) && toggle == 1) {
			Beep(500, 100);
			toggle = 0;
			state = "[Off]";
			system("cls");
			cout << "----------None's TriggerBot---------" << endl;
			cout << "Trigger : " << state << endl;
			
		}	

		for (int i = 1; i < 20; i++){

			address = bClient + oEntityList + (i * 0x10);
			ReadProcessMemory(hProcHandle, (int*)address, &ents, sizeof(ents), NULL);

			address = ents + oTeam;
			ReadProcessMemory(hProcHandle, (int*)address, &enemyTeam, sizeof(enemyTeam), NULL);

			address = LocalPlayer + oTeam;
			ReadProcessMemory(hProcHandle, (int*)address, &MyTeam, sizeof(MyTeam), NULL);
			
			address = LocalPlayer + oCrosshair;
			ReadProcessMemory(hProcHandle, (int*)address, &cross, sizeof(cross), NULL);

			if (cross >0 && cross <65 ) {
			
				address = bClient + oEntityList + (cross - 1) * oEntityLoopDistance;
				ReadProcessMemory(hProcHandle, (int*)address, &playerInCross, sizeof(playerInCross), NULL);

				address = playerInCross + oTeam;
				ReadProcessMemory(hProcHandle, (int*)address, &enemyTeam, sizeof(enemyTeam), NULL);

				address = playerInCross + oHealth;
				ReadProcessMemory(hProcHandle, (int*)address, &enemyHealth, sizeof(enemyHealth), NULL);

				if (enemyTeam != MyTeam && enemyTeam > 1 && enemyHealth > 0) {
					
					if (toggle == 1) {
						
						int attaque = 1;
						address = bClient + oAttaque;
						WriteProcessMemory(hProcHandle, (int*)address, &attaque, sizeof(attaque), NULL);
						Sleep(10);
						attaque = 4;
						WriteProcessMemory(hProcHandle, (int*)address, &attaque, sizeof(attaque), NULL);

					 }
					
				}
				
			}
			
			
		}

	} while (true);
    return 0;
}

