#include<iostream>
#include "Winsock2.h"
#include <string.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <tchar.h>
#include <chrono>
#pragma comment(lib, "WS2_32.lib")

using namespace std;


//Задание 
void SendMessageToServers(char* serverName, char* message) {
    HANDLE cH = CreateFileA(serverName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (cH == INVALID_HANDLE_VALUE) {
        cerr << "Не удалось создать файл. Ошибка: " << GetLastError() << endl;
    }
    else
    {
        cout << "Файл успешно создан" << endl;
    }

    DWORD bytesSended;
    if (!WriteFile(cH, message, strlen(message), &bytesSended, NULL)) {
        cerr << "Не удалось записать в почтовый ящик. Ошибка: " << GetLastError() << endl;
    }
    else {
        cout << "Сообщение успешно отправлено. Байты отправлены: " << bytesSended << endl;
    }

    CloseHandle(cH);
}

int main() {

    setlocale(LC_ALL, "rus");

    //HANDLE cH = CreateFile(TEXT("\\\\.\\mailslot\\Box"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //if (cH == INVALID_HANDLE_VALUE) {
    //    cerr << "Не удалось создать файл. Ошибка: " << GetLastError() << endl;
    //    return 1;
    //}
    //else {
    //    cout << "Файл успешно создан" << endl;
    //}

    //////Задание 9
    //auto start = chrono::high_resolution_clock::now();
   
    //for (int i = 1; i <= 1000; ++i) {
    //    char message[50];
    //    sprintf(message, "Hello %d", i);  

    //    DWORD bytesSent;
    //    if (!WriteFile(cH, message, strlen(message) + 1, &bytesSent, NULL)) {
    //        cerr << "Ошибка отправки сообщения: " << GetLastError() << " - " << message << endl;
    //        break;
    //    }

    //    cout << "Отправлено сообщение: " << message << endl;
    //    Sleep(5);  
    //}

    //auto end = chrono::high_resolution_clock::now();
    //chrono::duration<double> duration = end - start;
    //cout << "Время, необходимое для отправки 1000 сообщений: " << duration.count() << " секунд" << endl;
    //CloseHandle(cH);

    char nameServer[21] = "\\\\.\\mailslot\\Box";
    char message[50] = "Hello from Maislot-client";
    SendMessageToServers(nameServer, message);

   /* char nameServer2[40] = "\\\\Ancr33z\\mailslot\\Box";
    char message2[50] = "Hello from client";
    SendMessageToServers(nameServer2, message2);*/

    return 0;
}