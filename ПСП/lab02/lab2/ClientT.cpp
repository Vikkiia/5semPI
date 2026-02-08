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
    cout << "ClientT запускается..." << endl;

    SOCKET cC;                                                                                                                                                                              //дескриптор сокета
    WSADATA wsaData;                                                                                                                                                                        //Структура для хранения информации о Winsock

    try {
        
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)                                                                                                                                      //версия winsocket,  указатель на wsadata                               
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        
        if ((cC = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

    
        sockaddr_in serv;                                                                                                                                                                    // структура для адреса сервера                       
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);

        serv.sin_addr.s_addr = inet_addr("10.245.203.2"); 
        if (serv.sin_addr.s_addr == INADDR_NONE) {
            throw SetErrorMsgText("inet_addr: неверный адрес", WSAGetLastError());
        }

  
        cout << "Подключение к серверу ..." << endl;

        if (connect(cC, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("connect: ", WSAGetLastError());

        cout << "Подключено успешно!" << endl;

        char ibuf[100], obuf[100];
        int messageCount;

      
        cout << "Введите количество сообщений для отправки: ";
        cin >> messageCount;
        cin.ignore();

     
        auto start_time = high_resolution_clock::now();
        int successfulMessages = 0;

       
        for (int i = 0; i < messageCount; i++) {
           
            sprintf_s(obuf, sizeof(obuf), "Hello from Client %d", i + 1);
            int lobuf = (int)strlen(obuf) + 1;

            if (send(cC, obuf, lobuf, NULL) == SOCKET_ERROR) {
                cout << "Send сообщение об ошибке" << (i + 1) << ": " << WSAGetLastError() << endl;
                break;
            }

            cout << "Sent: " << obuf << endl;

           
            int libuf = recv(cC, ibuf, sizeof(ibuf) - 1, NULL);
            if (libuf == SOCKET_ERROR) {
                cout << "Ошибка получения сообщения " << (i + 1) << ": " << WSAGetLastError() << endl;
                break;
            }

            if (libuf == 0) {
                cout << "Сервер отключен" << endl;
                break;
            }

            ibuf[libuf] = '\0';
            cout << "Получено: " << ibuf << endl;
            successfulMessages++;
        }

        
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        double total_time = duration.count() / 1000.0;

      
        cout << "===============================" << endl;
       
        cout << "Время обмена: " << total_time << " seconds" << endl;
        
    
        const char* closeMsg = "CLOSE";
        send(cC, closeMsg, (int)strlen(closeMsg) + 1, NULL);

 
        Sleep(100);

     
        if (closesocket(cC) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());

        cout << "Отключение клиента завершено." << endl;
    }
    catch (string errorMsgText) {
        cout << endl << "Ошибка: " << errorMsgText << endl;
    }

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