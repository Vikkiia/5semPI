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

    PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(sd, TRUE, NULL, FALSE); 

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = sd;
    sa.bInheritHandle = FALSE;

    HANDLE Sh = CreateNamedPipe(
        L"\\\\.\\pipe\\Tube",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        5,
        1024, 1024,
        0,
        &sa
    );

    if (Sh == INVALID_HANDLE_VALUE) {
        std::cerr << SetErrorMessageText("CreateNamedPipe: ") << std::endl;
        return 1;
    }

    while (true) {
        BOOL connectResult = ConnectNamedPipe(Sh, nullptr);
        if (!connectResult) {
            DWORD err = GetLastError();
            if (err != ERROR_PIPE_CONNECTED) {
                cerr << SetErrorMessageText("ConnectNamedPipe: ") << endl;
                continue;
            }
        }

        cout << "Клиент подключен!" << endl;

        char buffer[256] = { 0 };
        DWORD bytesRead, bytesWritten;

        while (true) {
            if (!ReadFile(Sh, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
                DWORD err = GetLastError();
                if (err == ERROR_BROKEN_PIPE) {
                    cout << "Клиент отключился." << endl;
                    break;
                }
                else {
                    cout << SetErrorMessageText("ReadFile: ") << endl;
                    break;
                }
            }

            if (bytesRead == 0)
                break;

            buffer[bytesRead] = '\0';
            cout << "Сообщение от клиента: " << buffer << endl;

            if (!WriteFile(Sh, buffer, bytesRead, &bytesWritten, nullptr)) {
                cout << SetErrorMessageText("WriteFile: ") << endl;
                break;
            }
        }

        DisconnectNamedPipe(Sh);
        cout << "Ожидание нового клиента" << endl;
    }

    CloseHandle(Sh);
    return 0;
}