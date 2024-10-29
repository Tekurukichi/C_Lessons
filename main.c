#include <stdio.h>
#include <windows.h>

int main() {
    STARTUPINFOW su;
    PROCESS_INFORMATION pi;

    memset(&su, 0x00, sizeof(su));
    memset(&pi, 0x00, sizeof(pi));
    su.cb = sizeof(su);


    if (CreateProcessW(L"D:\\ProgramFiles\\Notepad++\\notepad++.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &su, &pi)) {
        printf("CreateProcess() worked\n");


        DWORD waitResult = WaitForSingleObject(pi.hProcess, 10000); // 10 секунд

        if (waitResult == WAIT_OBJECT_0) {
            printf("Finished process\n");

            DWORD exitCode;
            if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
                printf("End code: %d\n", exitCode);
            } else {
                printf("Can`t get end code\n");
            }
        } else if (waitResult == WAIT_TIMEOUT) {
            printf("Process execute too long. Terminate.\n");
            TerminateProcess(pi.hProcess, 0);
        } else {
            printf("Failed Timeout: %d\n", GetLastError());
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        printf("CreateProcess() failed :(, error: 0x%x \n", GetLastError());
    }

    return 0;
}