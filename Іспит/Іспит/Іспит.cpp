#include <iostream>
#include <Windows.h>

int main() {
    HANDLE hFile = CreateFile(
        L"input.txt",             
        GENERIC_READ,             
        FILE_SHARE_READ,          
        NULL,                     
        OPEN_EXISTING,            
        FILE_ATTRIBUTE_NORMAL,    
        NULL                      
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Помилка відкриття файлу: " << GetLastError() << std::endl;
        return 1;
    }

   
    DWORD fileSize = GetFileSize(hFile, NULL);

    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Помилка отримання розміру файлу: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

   
    HANDLE hMapping = CreateFileMapping(
        hFile,                    
        NULL,                     
        PAGE_READONLY,            
        0,                        
        fileSize,                 
        NULL                      
    );


    if (hMapping == NULL) {
        std::cerr << "Помилка створення відображення: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    
    LPVOID fileData = MapViewOfFile(
        hMapping,                
        FILE_MAP_READ,           
        0,                       
        0,                       
        0                        
    );

    if (fileData == NULL) {
        std::cerr << "Помилка відображення файлу в пам'ять: " << GetLastError() << std::endl;
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return 1;
    }

    
    int charCount = 0;
    for (DWORD i = 0; i < fileSize; ++i) {
        if (static_cast<char*>(fileData)[i] != '\0')
            charCount++;
    }


    
    std::cout << "Кількість символів у файлі: " << charCount << std::endl;

    UnmapViewOfFile(fileData);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    return 0;
}