#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 80
#define BUFFER_SIZE 2048

const char *html_response = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n"
"\r\n"
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <title>Simple Winsock Server</title>\n"
"</head>\n"
"<body>\n"
"    <h1>Hello from Winsock Server!</h1>\n"
"    <p>This is a simple HTML page served by a C Winsock server.</p>\n"
"</body>\n"
"</html>";

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized\n");

    // Создание сокета
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket created\n");

    // Настройка сервера
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Bind done\n");

    // Ожидание подключений
    listen(server_socket, 3);
    printf("Waiting for incoming connections...\n");

    client_len = sizeof(struct sockaddr_in);

    while (1) {
        // Принятие подключения
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len)) == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }
        printf("Connection accepted\n");

        // Получение данных от клиента
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Received request:\n%s\n", buffer);

            // Проверяем, запрашивается ли index.html
            if (strstr(buffer, "GET /index.html") != NULL || strstr(buffer, "GET / ") != NULL) {
                // Отправка HTML-страницы
                send(client_socket, html_response, strlen(html_response), 0);
                printf("HTML page sent\n");
            } else {
                // Отправка 404 для других запросов
                const char *not_found = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(client_socket, not_found, strlen(not_found), 0);
                printf("404 Not Found sent\n");
            }
        }

        // Закрытие соединения
        closesocket(client_socket);
        printf("Connection closed\n\n");
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}