#include <iostream>
#include <windows.h>
#include <tchar.h>

#define BUFFER_SIZE 4096

int main() {
    
    LPCTSTR inputFileName = _T("input.txt");
    LPCTSTR outputFileName = _T("output.txt");

    HANDLE hInputFile = INVALID_HANDLE_VALUE;
    HANDLE hOutputFile = INVALID_HANDLE_VALUE;
    DWORD bytesRead;
    DWORD bytesWritten;
    BYTE buffer[BUFFER_SIZE];
    BOOL success = FALSE;

   
    hInputFile = CreateFile(
        inputFileName,
        GENERIC_READ,
        0, 
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hInputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening input file: " << GetLastError() << std::endl;
        return 1;
    }

   
    hOutputFile = CreateFile(
        outputFileName,
        GENERIC_WRITE,
        0, 
        NULL,
        CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hOutputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating/opening output file: " << GetLastError() << std::endl;
        CloseHandle(hInputFile);
        return 1;
    }

    
    do {
        success = ReadFile(
            hInputFile,
            buffer,
            BUFFER_SIZE,
            &bytesRead,
            NULL
        );

        if (!success && GetLastError() != ERROR_HANDLE_EOF) {
            std::cerr << "Error reading from input file: " << GetLastError() << std::endl;
            break;
        }

        if (bytesRead > 0) {
            success = WriteFile(
                hOutputFile,
                buffer,
                bytesRead,
                &bytesWritten,
                NULL
            );

            if (!success) {
                std::cerr << "Error writing to output file: " << GetLastError() << std::endl;
                break;
            }
        }
    } while (success && bytesRead > 0);

   
    if (hInputFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hInputFile);
    }
    if (hOutputFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hOutputFile);
    }

    if (success) {
        std::cout << "File copied successfully." << std::endl;
        return 0;
    }
    else {
        return 1;
    }
}
