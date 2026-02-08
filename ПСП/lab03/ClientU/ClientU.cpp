#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <locale.h>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(int code);

int main() {
    setlocale(LC_ALL, "Russian");
    SOCKET cC;
    WSADATA wsaData;

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw SetErrorMsgText(WSAGetLastError());
        }

        if ((cC = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
            throw SetErrorMsgText(WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr("10.219.157.2");
        //serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        int messageCount;
        cout << "Введите количество сообщений: ";
        cin >> messageCount;
      

        cout << "Запуск с " << messageCount << " сообщениями..." << endl;

        clock_t start = clock();

        for (int i = 1; i <= messageCount; i++) {
            string msg = "Hello from Client" + to_string(i);

            cout << "Отправлено: " << msg << endl;

            if (sendto(cC, msg.c_str(), (int)msg.length() + 1, NULL,
                (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
                throw SetErrorMsgText(WSAGetLastError());
            }

            /*char buf[1024];
            SOCKADDR_IN fromServ;
            memset(&fromServ, 0, sizeof(fromServ));
            int fromLen = sizeof(fromServ);*/
            /*int recvCount = 0;
            if ((recvCount = recvfrom(cC, buf, sizeof(buf) - 1, NULL,
                (sockaddr*)&fromServ, &fromLen)) == SOCKET_ERROR) {
                throw SetErrorMsgText(WSAGetLastError());
            }
            buf[recvCount] = '\0';

           cout << "Получено: " << buf << endl;*/
        }

        string emptyMsg = "";
        if (sendto(cC, emptyMsg.c_str(), (int)emptyMsg.length() + 1, NULL,
            (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText(WSAGetLastError());
        }

        clock_t end = clock();
        double time = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "Время для " << messageCount << " сообщений: " << time << " секунд" << endl;
       

        if (closesocket(cC) == SOCKET_ERROR) {
            throw SetErrorMsgText(WSAGetLastError());
        }
        if (WSACleanup() == SOCKET_ERROR) {
            throw SetErrorMsgText(WSAGetLastError());
        }
    }
    catch (string errorMsgText) {
        cout << endl << errorMsgText;
    }
    return 0;
}

string GetErrorMsgText(int code)
{
    string msgText;
    switch (code)
    {
    case WSAEINTR:             msgText = "Работа функции прервана"; break;
    case WSAEACCES:            msgText = "Разрешение отвергнуто"; break;
    case WSAEFAULT:            msgText = "Ошибочный адрес"; break;
    case WSAEINVAL:            msgText = "Ошибка в аргументе"; break;
    case WSAEMFILE:            msgText = "Слишком много файлов открыто"; break;
    case WSAEWOULDBLOCK:       msgText = "Ресурс временно недоступен"; break;
    case WSAEINPROGRESS:       msgText = "Операция в процессе развития"; break;
    case WSAEALREADY:          msgText = "Операция уже выполняется"; break;
    case WSAENOTSOCK:          msgText = "Сокет задан неправильно"; break;
    case WSAEDESTADDRREQ:      msgText = "Требуется адрес расположения"; break;
    case WSAEMSGSIZE:          msgText = "Сообщение слишком длинное"; break;
    case WSAEPROTOTYPE:        msgText = "Неправильный тип протокола для сокета"; break;
    case WSAENOPROTOOPT:       msgText = "Ошибка в опции протокола"; break;
    case WSAEPROTONOSUPPORT:   msgText = "Протокол не поддерживается"; break;
    case WSAESOCKTNOSUPPORT:   msgText = "Тип сокета не поддерживается"; break;
    case WSAEOPNOTSUPP:        msgText = "Операция не поддерживается"; break;
    case WSAEPFNOSUPPORT:      msgText = "Тип протоколов не поддерживается"; break;
    case WSAEAFNOSUPPORT:      msgText = "Тип адресов не поддерживается протоколом"; break;
    case WSAEADDRINUSE:        msgText = "Адрес уже используется"; break;
    case WSAEADDRNOTAVAIL:     msgText = "Запрошенный адрес не может быть использован"; break;
    case WSAENETDOWN:          msgText = "Сеть отключена"; break;
    case WSAENETUNREACH:       msgText = "Сеть не достижима"; break;
    case WSAENETRESET:         msgText = "Сеть разорвала соединение"; break;
    case WSAECONNABORTED:      msgText = "Программный отказ связи"; break;
    case WSAECONNRESET:        msgText = "Связь восстановлена"; break;
    case WSAENOBUFS:           msgText = "Не хватает памяти для буферов"; break;
    case WSAEISCONN:           msgText = "Сокет уже подключен"; break;
    case WSAENOTCONN:          msgText = "Сокет не подключен"; break;
    case WSAESHUTDOWN:         msgText = "Нельзя выполнить send: сокет завершил работу"; break;
    case WSAETIMEDOUT:         msgText = "Закончился отведенный интервал времени"; break;
    case WSAECONNREFUSED:      msgText = "Соединение отклонено"; break;
    case WSAEHOSTDOWN:         msgText = "Хост в неработоспособном состоянии"; break;
    case WSAEHOSTUNREACH:      msgText = "Нет маршрута для хоста"; break;
    case WSAEPROCLIM:          msgText = "Слишком много процессов"; break;
    case WSASYSNOTREADY:       msgText = "Сеть не доступна"; break;
    case WSAVERNOTSUPPORTED:   msgText = "Данная версия недоступна"; break;
    case WSANOTINITIALISED:    msgText = "Не выполнена инициализация WS2_32.DLL"; break;
    case WSAEDISCON:           msgText = "Выполняется отключение"; break;
    case WSATYPE_NOT_FOUND:    msgText = "Класс не найден"; break;
    case WSAHOST_NOT_FOUND:    msgText = "Хост не найден"; break;
    case WSATRY_AGAIN:         msgText = "Неавторизированный хост не найден"; break;
    case WSANO_RECOVERY:       msgText = "Неопределенная ошибка"; break;
    case WSANO_DATA:           msgText = "Нет записи запрошенного типа"; break;
    case WSA_INVALID_HANDLE:   msgText = "Указанный дескриптор события с ошибкой"; break;
    case WSA_INVALID_PARAMETER:msgText = "Один или более параметров с ошибкой"; break;
    case WSA_IO_INCOMPLETE:    msgText = "Объект ввода-вывода не в сигнальном состоянии"; break;
    case WSA_IO_PENDING:       msgText = "Операция завершится позже"; break;
    case WSA_NOT_ENOUGH_MEMORY:msgText = "Не достаточно памяти"; break;
    case WSA_OPERATION_ABORTED:msgText = "Операция отвергнута"; break;
    case WSASYSCALLFAILURE:    msgText = "Аварийное завершение системного вызова"; break;
    default:                   msgText = "ERROR: Неизвестная ошибка"; break;
    };
    return msgText;
}

string SetErrorMsgText(int code) {
    return GetErrorMsgText(code) + ": " + to_string(code);
}