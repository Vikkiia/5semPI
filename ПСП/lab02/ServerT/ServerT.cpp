#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <tchar.h>
#include <chrono>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;
using namespace std::chrono;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);

int main()
{
    setlocale(LC_ALL, "rus");
    cout << "ServerT начинается..." << endl;

    WSADATA wsaData;
    SOCKET sS, cS;
    sockaddr_in serv, client;
    int lc = sizeof(client);
    char ibuf[100];
    int libuf = 0;
    int clientCount = 0;

    try {
     
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        
        if ((sS = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

   
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;

        
        if (bind(sS, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind: ", WSAGetLastError());

      
        if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw SetErrorMsgText("listen: ", WSAGetLastError());

        cout << "Сервер ожидает подключений на порту 2000..." << endl;

        
        while (true) {
           
            if ((cS = accept(sS, (sockaddr*)&client, &lc)) == INVALID_SOCKET)
                throw SetErrorMsgText("accept: ", WSAGetLastError());

            clientCount++;
            cout << "\n=== Клиент " << clientCount << " соединен ===" << endl;
            cout << "Клиент IP: " << inet_ntoa(client.sin_addr) << endl;
            cout << "Клиент port: " << ntohs(client.sin_port) << endl;

            high_resolution_clock::time_point start_time;
            high_resolution_clock::time_point end_time;
            int messageCount = 0;
            bool firstMessage = true;
            bool clientActive = true;

       
            while (clientActive) {
            
                libuf = recv(cS, ibuf, sizeof(ibuf), NULL);

                if (libuf == SOCKET_ERROR) {
                    cout << "Recv ошибка: " << WSAGetLastError() << endl;
                    break;
                }

                if (libuf == 0) {
                    cout << "Клиент отключен" << endl;
                    break;
                }

                ibuf[libuf] = '\0';

               
                if (firstMessage) {
                    start_time = high_resolution_clock::now();
                    firstMessage = false;
                }

  
                if (strcmp(ibuf, "CLOSE") == 0) {
                    end_time = high_resolution_clock::now();
                    break;
                }

                messageCount++;
                cout << "Сообщение " << messageCount << " от клиента: " << ibuf << endl;

                
                if (send(cS, ibuf, libuf, NULL) == SOCKET_ERROR) {
                    cout << "Send ошибка: " << WSAGetLastError() << endl;
                    end_time = high_resolution_clock::now();
                    break;
                }

            }

           
            if (firstMessage) {
                start_time = high_resolution_clock::now();
                end_time = start_time;
            }
            else if (messageCount > 0 && !clientActive) {
                end_time = high_resolution_clock::now();
            }

            auto duration = duration_cast<milliseconds>(end_time - start_time);
            double total_time = duration.count() / 1000.0;

            cout << "=================================" << endl;
            cout << "Общее время: " << total_time << " seconds" << endl;
            
        

            
            if (closesocket(cS) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        }

    
        if (closesocket(sS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText) {
        cout << endl << "Ошибка: " << errorMsgText << endl;
    }

    cout << "Server shutdown." << endl;
    system("pause");
    return 0;
}

string GetErrorMsgText(int code)
{
    string msgText;
    switch (code) {
    case WSAEINTR: msgText = "Работа функции прервана"; break;
    case WSAEACCES: msgText = "Разрешение отвергнуто"; break;
    case WSAEFAULT: msgText = "Ошибочный адрес"; break;
    case WSAENETDOWN: msgText = "Сеть отключена"; break;
    case WSAENETUNREACH: msgText = "Сеть недостижима"; break;
    case WSAENETRESET: msgText = "Сеть разорвала соединение"; break;
    case WSAECONNABORTED: msgText = "Программный отказ связи"; break;
    case WSAECONNRESET: msgText = "Связь восстановлена"; break;
    case WSAENOBUFS: msgText = "Не хватает памяти для буферов"; break;
    case WSAEISCONN: msgText = "Сокет уже подключен"; break;
    case WSAENOTCONN: msgText = "Сокет не подключен"; break;
    case WSAESHUTDOWN: msgText = "Нельзя выполнить send: сокет завершил работу"; break;
    case WSAETIMEDOUT: msgText = "Закончился отведенный интервал  времени"; break;
    case WSAECONNREFUSED: msgText = "Соединение отклонено"; break;
    case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии"; break;
    case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста"; break;
    case WSASYSCALLFAILURE: msgText = "Сбой системного вызова"; break;
    default: msgText = "Неизвестная ошибка: " + to_string(code); break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
    return msgText + " (" + to_string(code) + ") - " + GetErrorMsgText(code);
}