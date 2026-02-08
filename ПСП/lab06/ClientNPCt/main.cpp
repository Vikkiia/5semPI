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

    int N = 0;
    cout << "N: ";
    cin >> N;

    for (int i = 1; i <= N; i++) {
        string msg = "hello world " + to_string(i);

        char buffer[256] = { 0 };
        DWORD bytesRead = 0;

        BOOL ok = CallNamedPipe(
            pipeName,                     
            (LPVOID)msg.c_str(),         
            (DWORD)msg.size(),            
            buffer,                      
            sizeof(buffer) - 1,           
            &bytesRead,                   
            NMPWAIT_WAIT_FOREVER         
        );

        if (!ok) {
            cout << SetErrorMessageText("CallNamedPipe: ") << endl;
            return 1;
        }

        buffer[bytesRead] = '\0';
        cout << "Ответ от сервера: " << buffer << endl;
    }

    return 0;
}