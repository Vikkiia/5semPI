#include <windows.h>
#include<string>
#include <sstream>
#include<iostream>

using namespace std;

string GetErrorMessage()
{
    DWORD code = GetLastError();
    if (code == 0)
        return "No error";

    LPVOID msgBuffer;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuffer,
        0,
        NULL
    );

    std::ostringstream oss;
    if (size == 0) {
        oss << "Unknown error code: " << code;
    }
    else {
        oss << "Error " << code << ": " << (LPSTR)msgBuffer;
    }

    LocalFree(msgBuffer);
    return oss.str();
}
string SetErrorMessageText(string errorMSg) {
    return errorMSg + GetErrorMessage();
}



int main() {

    setlocale(LC_ALL, "rus");

    LPCWSTR pipeName = L"\\\\DESKTOP-81VFTB7\\pipe\\Tube";

    HANDLE hPipe = CreateFile(
        pipeName,                
        GENERIC_READ | GENERIC_WRITE, 
        0,                       
        nullptr,                 
        OPEN_EXISTING,           
        0,                       
        nullptr                  
    );


    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << SetErrorMessageText("CreateFile: ") << endl;
        return 1;
    }

    int N = 0;
    cout << "N: "; cin >> N;

    for (int i = 1; i <= N; i++) {

        string msg = "hello!!!!" + to_string(i);
        DWORD written;
        if (!WriteFile(hPipe, msg.c_str(), (DWORD)msg.size(), &written, nullptr)) {
            cout << SetErrorMessageText("WriteFile: ") << endl;
            CloseHandle(hPipe);
            return 1;
        }

        char buffer[256] = { 0 };
        DWORD read;

        if (!ReadFile(hPipe, buffer, sizeof(buffer) - 1, &read, nullptr)) {
            cout << SetErrorMessageText("ReadFile: ") << endl;
            CloseHandle(hPipe);
            return 1;
        }



        buffer[read] = '\0';
        cout << "message from server: " << buffer << endl;


    }
    if (!CloseHandle(hPipe)) {
        cout << SetErrorMessageText("CloseHandle: ") << endl;
        return 1;
    }
}