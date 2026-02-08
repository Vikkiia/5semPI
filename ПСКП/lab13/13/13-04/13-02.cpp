#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"  
#define PORT 3000 
#define BUFFER_SIZE 1024  

int main() {
    setlocale(LC_ALL, "RUS");
    SetConsoleOutputCP(1251);

    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = "HIII!";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка WSAStartup: " << WSAGetLastError() << std::endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения к серверу: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Подключено к серверу!" << std::endl;

    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Ошибка отправки данных: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Сообщение отправлено: " << buffer << std::endl;

    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Ответ сервера: " << buffer << std::endl;
    }
    else {
        std::cerr << "Ошибка при получении данных: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
