#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <winsock2.h>
#include <windows.h> 
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;

#define PORT 3000
#define BUFFER_SIZE 1024

int main() {
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "RUS");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Ошибка создания сокета");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки");
        return EXIT_FAILURE;
    }

    listen(server_fd, 5);
    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true) {  
       
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Ошибка принятия соединения");
            continue;  
        }

        std::cout << "Клиент подключился" << std::endl;

        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                std::cout << "Клиент отключился" << std::endl;
                break;  
            }

            std::cout << "Получено: " << buffer << std::endl;
            std::string response = "ECHO: " + std::string(buffer);
            send(client_fd, response.c_str(), response.length(), 0);
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
