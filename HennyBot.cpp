#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
using namespace std;

void getProcessHandle(HANDLE* handle) {
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
                                wcout << modName;
                                *handle = hProcess;
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
    cout << "HennyBot\n";

    HANDLE hProcess = NULL;

    getProcessHandle(&hProcess);

    if (hProcess == NULL) {
        cout << "Target was not found.\n";
        return 1;
    }

    return 0;
}