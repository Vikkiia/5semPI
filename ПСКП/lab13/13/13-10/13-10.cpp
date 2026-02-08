#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 3000
#define BUFFER_SIZE 1024

int main() {
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    SOCKET udpSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = "hIHIHIH!";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка WSAStartup: " << WSAGetLastError() << std::endl;
        return 1;
    }

    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);

    sendto(udpSocket, buffer, strlen(buffer), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "Сообщение отправлено: " << buffer << std::endl;

    sockaddr_in fromAddr;
    int fromAddrSize = sizeof(fromAddr);
    int bytesReceived = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&fromAddr, &fromAddrSize);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Ответ сервера: " << buffer << std::endl;
    }
    else {
        std::cerr << "Ошибка при получении данных: " << WSAGetLastError() << std::endl;
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}
