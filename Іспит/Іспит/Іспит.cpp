#include <iostream>
#include <windows.h>
#include <fstream>

int main() {
    const wchar_t* filename = L"example.txt"; 

    
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << "This is a test string.\n";
        outfile << "Another line for testing.\n";
        outfile.close();
    }

    
    HANDLE hFile = CreateFileW(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Could not open file. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        std::cerr << "Could not get file size. Error code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

   
    if (fileSize.QuadPart == 0) {
        std::cout << "File is empty." << std::endl;
        CloseHandle(hFile);
        return 0;
    }

  
    HANDLE hMapping = CreateFileMappingW(
        hFile,
        NULL,
        PAGE_READONLY,
        0,
        fileSize.LowPart,
        NULL);

    if (hMapping == NULL) {
        std::cerr << "Could not create file mapping object. Error code: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }


    LPVOID lpBase = MapViewOfFile(
        hMapping,
        FILE_MAP_READ,
        0,
        0,
        fileSize.LowPart);

    if (lpBase == NULL) {
        std::cerr << "Could not map view of file. Error code: " << GetLastError() << std::endl;
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return 1;
    }

   
    char* fileContent = static_cast<char*>(lpBase);
    size_t characterCount = 0;
    for (DWORD i = 0; i < fileSize.LowPart; ++i) {
        characterCount++;
    }

   
    std::cout << "File: " << filename << std::endl;
    std::cout << "Number of characters: " << characterCount << std::endl;

  
    if (!UnmapViewOfFile(lpBase)) {
        std::cerr << "Could not unmap view of file. Error code: " << GetLastError() << std::endl;
    }

   
    if (!CloseHandle(hMapping)) {
        std::cerr << "Could not close file mapping object. Error code: " << GetLastError() << std::endl;
    }

    
    if (!CloseHandle(hFile)) {
        std::cerr << "Could not close file handle. Error code: " << GetLastError() << std::endl;
    }

    return 0;
}
