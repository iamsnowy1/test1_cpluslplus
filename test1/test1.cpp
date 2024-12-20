#include <windows.h>
#include <iostream>

void handleError(const wchar_t* errorMessage) {
    std::wcerr << errorMessage << L" Error: " << GetLastError() << std::endl;
}

int main() {
    const wchar_t* inputFileName = L"C:\\Users\\verba\\OneDrive\\Робочий стіл\\Main Dir\\study\\kurs3\\prog\\test1\\input.txt";
    const wchar_t* outputFileName = L"C:\\Users\\verba\\OneDrive\\Робочий стіл\\Main Dir\\study\\kurs3\\prog\\test1\\test123\\output.txt";

    // Open the input file
    HANDLE hInputFile = CreateFile(
        inputFileName,            // File name
        GENERIC_READ,             // Desired access
        0,                        // Share mode
        NULL,                     // Security attributes
        OPEN_EXISTING,            // Creation disposition
        FILE_ATTRIBUTE_NORMAL,    // Flags and attributes
        NULL                      // Template file
    );

    if (hInputFile == INVALID_HANDLE_VALUE) {
        handleError(L"Failed to open input file");
        return 1;
    }

    // Create the output file
    HANDLE hOutputFile = CreateFile(
        outputFileName,           // File name
        GENERIC_WRITE,            // Desired access
        0,                        // Share mode
        NULL,                     // Security attributes
        CREATE_ALWAYS,            // Creation disposition
        FILE_ATTRIBUTE_NORMAL,    // Flags and attributes
        NULL                      // Template file
    );

    if (hOutputFile == INVALID_HANDLE_VALUE) {
        handleError(L"Failed to create output file");
        CloseHandle(hInputFile);
        return 1;
    }

    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;

    // Read from the input file and write to the output file
    while (ReadFile(hInputFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        if (!WriteFile(hOutputFile, buffer, bytesRead, &bytesWritten, NULL)) {
            handleError(L"Failed to write to output file");
            CloseHandle(hInputFile);
            CloseHandle(hOutputFile);
            return 1;
        }
    }

    if (GetLastError() != ERROR_HANDLE_EOF) {
        handleError(L"Error reading input file");
    }

    // Close file handles
    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);

    std::wcout << L"File copy operation completed successfully." << std::endl;
    return 0;
}