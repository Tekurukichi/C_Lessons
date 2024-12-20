#include <iostream>
#include <Windows.h>

int main() {
	HANDLE hInputFile = CreateFile(
		L"input.txt",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hInputFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Помилка відкриття файлу для читання: " << GetLastError() << std::endl;
		return 1;
	}


	HANDLE hOutputFile = CreateFile(
		L"output.txt",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hOutputFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Помилка відкриття файлу для запису: " << GetLastError() << std::endl;
		CloseHandle(hInputFile);
		return 1;
	}


	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	char buffer[4096];

	while (ReadFile(hInputFile, buffer, sizeof(buffer), &dwBytesRead, NULL) && dwBytesRead > 0) {
		if (!WriteFile(hOutputFile, buffer, dwBytesRead, &dwBytesWritten, NULL) || dwBytesWritten != dwBytesRead) {
			std::cerr << "Помилка запису у файл: " << GetLastError() << std::endl;
			break;
		}
	}



	CloseHandle(hInputFile);
	CloseHandle(hOutputFile);

	std::cout << "Файл успішно скопійовано." << std::endl;

	return 0;
}