#include <iostream>
#include <windows.h>
#include <tchar.h>

#define BUFFER_SIZE 4096

int main() {
    // Path to the input and output files
    LPCTSTR inputFileName = _T("input.txt");
    LPCTSTR outputFileName = _T("output.txt");

    HANDLE hInputFile = INVALID_HANDLE_VALUE;
    HANDLE hOutputFile = INVALID_HANDLE_VALUE;
    DWORD bytesRead;
    DWORD bytesWritten;
    BYTE buffer[BUFFER_SIZE];
    BOOL success = FALSE;

    // Open the existing file for reading
    hInputFile = CreateFile(
        inputFileName,
        GENERIC_READ,
        0, // Do not share
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hInputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening input file: " << GetLastError() << std::endl;
        return 1;
    }

    // Create or open the output file for writing
    hOutputFile = CreateFile(
        outputFileName,
        GENERIC_WRITE,
        0, // Do not share
        NULL,
        CREATE_ALWAYS, // Create a new file or overwrite an existing one
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hOutputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating/opening output file: " << GetLastError() << std::endl;
        CloseHandle(hInputFile);
        return 1;
    }

    // Read from the input file and write to the output file
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

    // Close file handles
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
