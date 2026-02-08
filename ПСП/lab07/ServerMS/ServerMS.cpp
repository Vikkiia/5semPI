#include <iostream>
#include "Winsock2.h"
#include <string.h>
#include <ws2tcpip.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <tchar.h>
using namespace std;
#pragma comment(lib, "WS2_32.lib")

string SetErrorMsgText(string msg, int errorNumber) {
    string result = "";
    switch (errorNumber)
    {
    case WSAEINTR: result = msg + "Работа функции прервана WSAITR"; break;
    case WSAEACCES: result = msg + "Разрешение отвергнутоWSAEACCES"; break;
    case WSAEFAULT: result = msg + "Ошибочный адрес  WSAEFAULT"; break;
    case WSAEINVAL: result = msg + "Ошибка в аргументеWSAEINVAL"; break;
    case WSAEMFILE: result = msg + "Слишком много файлов открытоWSAEMFILE"; break;
    case WSAEWOULDBLOCK: result = msg + "Ресурс временно недоступенWSAEWOULDBLOCK"; break;
    case WSAEINPROGRESS: result = msg + "Операция в процессе развитияWSAEINPROGRESS"; break;
    case WSAEALREADY: result = msg + "Операция уже выполняется WSAEALREADY"; break;
    case WSAENOTSOCK: result = msg + "Сокет задан неправильно  WSAENOTSOCK "; break;
    case WSAEDESTADDRREQ: result = msg + "Требуется адрес расположения WSAEDESTADDRREQ "; break;
    case WSAEMSGSIZE: result = msg + "Сообщение слишком длинное "; break;
    case WSAEPROTOTYPE: result = msg + "Неправильный тип протокола для сокета "; break;
    case WSAENOPROTOOPT: result = msg + "Ошибка в опции протокола"; break;
    case WSAEPROTONOSUPPORT: result = msg + "Протокол не поддерживается"; break;
    case WSAESOCKTNOSUPPORT: result = msg + "Тип сокета не поддерживается "; break;
    case WSAEOPNOTSUPP: result = msg + "Операция не поддерживается"; break;
    case WSAEPFNOSUPPORT: result = msg + "Тип протоколов не поддерживается "; break;
    case WSAEAFNOSUPPORT: result = msg + "Тип адресов не поддерживается протоколом"; break;
    case WSAEADDRINUSE: result = msg + "Адрес уже используется "; break;
    case WSAEADDRNOTAVAIL: result = msg + "Запрошенный адрес не может быть использован"; break;
    case WSAENETDOWN: result = msg + "Сеть отключена "; break;
    case WSAENETUNREACH: result = msg + "Сеть не достижима"; break;
    case WSAENETRESET: result = msg + "Сеть разорвала соединение"; break;
    case WSAECONNABORTED: result = msg + "Программный отказ связи"; break;
    case WSAECONNRESET: result = msg + "Связь восстановлена "; break;
    case WSAENOBUFS: result = msg + "Не хватает памяти для буферов"; break;
    case WSAEISCONN: result = msg + "Сокет уже подключен"; break;
    case WSAENOTCONN: result = msg + "Сокет не подключен"; break;
    case WSAESHUTDOWN: result = msg + "Нельзя выполнить send: сокет завершил работу"; break;
    case WSAETIMEDOUT: result = msg + "Закончился отведенный интервал  времени"; break;
    case WSAECONNREFUSED: result = msg + "Соединение отклонено"; break;
    case WSAEHOSTDOWN: result = msg + "Хост в неработоспособном состоянии"; break;
    case WSAEHOSTUNREACH: result = msg + "Нет маршрута для хоста "; break;
    case WSAEPROCLIM: result = msg + "Слишком много процессов "; break;
    case WSASYSNOTREADY: result = msg + "Сеть не доступна"; break;
    case WSAVERNOTSUPPORTED: result = msg + "Данная версия недоступна "; break;
    case WSANOTINITIALISED: result = msg + "Не выполнена инициализация WS2_32.DLL"; break;
    case WSAEDISCON: result = msg + "Выполняется отключение"; break;
    case WSATYPE_NOT_FOUND: result = msg + "Класс не найден "; break;
    case WSAHOST_NOT_FOUND: result = msg + "Хост не найден"; break;
    case WSATRY_AGAIN: result = msg + "Неавторизированный хост не найден "; break;
    case WSANO_RECOVERY: result = msg + "Неопределенная  ошибка"; break;
    case WSANO_DATA: result = msg + "Нет записи запрошенного типа "; break;
    case WSA_INVALID_HANDLE: result = msg + "Указанный дескриптор события  с ошибкой"; break;
    case WSA_INVALID_PARAMETER: result = msg + "Один или более параметров с ошибкой"; break;
    case WSA_IO_INCOMPLETE: result = msg + "Объект ввода-вывода не в сигнальном состоянии"; break;
    case WSA_IO_PENDING: result = msg + "Операция завершится позже"; break;
    case WSA_NOT_ENOUGH_MEMORY: result = msg + "Не достаточно памяти "; break;
    case WSA_OPERATION_ABORTED: result = msg + "Операция отвергнута"; break;
    case WSASYSCALLFAILURE: result = msg + "Аварийное завершение системного вызова"; break;
    default:
        break;
    }
    return result;
}


int main() {
    setlocale(LC_ALL, "rus");

    HANDLE sH = CreateMailslot(TEXT("\\\\.\\mailslot\\Box"), 500, 180000, NULL);
    cout << "Прослушивание сервера..." << endl;
    if (sH == INVALID_HANDLE_VALUE) {
        throw SetErrorMsgText("create: ", GetLastError());
    }

    char rbuf[200];
    DWORD bytesRead;
    if (!ReadFile(sH, rbuf, sizeof(rbuf) - 1, &bytesRead, NULL)) {
        DWORD error = GetLastError();
        if (error == ERROR_SEM_TIMEOUT) {
            cout << "Истекло время ожидания при чтении из Mailslot." << endl;
        }
        else {
            throw SetErrorMsgText("read: ", GetLastError());
        }
    }
    else {
        rbuf[bytesRead] = '\0';
        cout << bytesRead << endl;
        cout << "Получено сообщение: " << rbuf << endl;
    }   

    //Задание 9
    while (true) {
    if (!ReadFile(sH, rbuf, sizeof(rbuf), &bytesRead, NULL)) {
        cerr << "Ошибка чтения из mailslot: " << GetLastError() << endl;
        continue;
    }
    rbuf[bytesRead] = '\0';
    cout << "Получено сообщение: " << rbuf << endl;
    }

    CloseHandle(sH);    

    return 0;
}