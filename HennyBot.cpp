#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
using namespace std;

/* offsets */
const int GAME_POINTER_OFFSET = 0x1F440;
const int PLAYER_HEALTH_OFFSET = 0x4;

void getProcessHandle(HANDLE *pHandle, HMODULE *pHmod) {
    const wchar_t targetName[] = TEXT("BloogGame.exe");
    DWORD processIds[1024], cbNeeded;
    unsigned int i;

    if (EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
        DWORD numProcesses = cbNeeded / sizeof(DWORD);
        wchar_t modName[MAX_PATH] = TEXT("<name>");

        for (i = 0; i < numProcesses; i++) {
            if (processIds[i] != 0) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                    PROCESS_VM_READ, FALSE, processIds[i]);

                if (hProcess != NULL) {
                    HMODULE hMod;
                    DWORD cbNeeded;

                    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                        if (GetModuleBaseName(hProcess, hMod, modName, sizeof(modName) / sizeof(wchar_t))) {
                            if (wcscmp(targetName, modName) == 0) {
                                wcout << modName << '\n';
                                *pHandle = hProcess;
                                *pHmod = hMod;
                                return;
                            }
                        }
                    }
                }

                CloseHandle(hProcess);
            };
        }
    }
}

int main()
{
    HANDLE hProcess = NULL;
    HMODULE hMod = NULL;

    getProcessHandle(&hProcess, &hMod);

    if (hProcess == NULL) {
        cout << "Target was not found.\n";
        return 1;
    }

    cout << "Base addr\t" << hMod << '\n';

    void* gameObjAddr = hMod + GAME_POINTER_OFFSET;

    cout << "Game obj addr\t" << gameObjAddr;

    return 0;
}