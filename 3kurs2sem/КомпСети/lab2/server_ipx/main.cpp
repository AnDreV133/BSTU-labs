#include <windows.h>
#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <wsipx.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <chrono>
#include <thread>

#define CLIENT_REQUEST_SIZE 512
#define IMAGE_PART_SIZE 400
#define IPX_SOCKET (0x8060)

SOCKADDR_IPX name = {};

FILE *source;

void send_loop(SOCKET socket_server)
{
    bool should_run = true;
    int bytes_read;

    while (should_run)
    {
        std::string input;
        printf("Input Y to send file\nType anything else to stop server: ");
        std::cin >> input;

        if (input != "Y")
        {
            break;
        }

        SOCKADDR_IPX client_sockaddr = {};
        client_sockaddr.sa_family = AF_IPX;
        memset(client_sockaddr.sa_netnum, 0, 4);
        memset(client_sockaddr.sa_nodenum, 0xFF, 6);
        client_sockaddr.sa_socket = htons(IPX_SOCKET);

        int client_sockaddr_size = sizeof(client_sockaddr);

        int packages_success = 0, packages_error = 0;
        fseek(source, 0, SEEK_SET);

        auto start_send_time = std::chrono::high_resolution_clock::now();
        char image_buffer[IMAGE_PART_SIZE];

        while ((bytes_read = fread(image_buffer, sizeof(char), IMAGE_PART_SIZE, source)))
        {
            if (sendto(socket_server,
                       image_buffer,
                       bytes_read,
                       0,
                       (sockaddr *)&client_sockaddr,
                       client_sockaddr_size) == SOCKET_ERROR)
            {
                packages_error++;
            }
            else
            {
                packages_success++;
            }
        }
        auto end_send_time = std::chrono::high_resolution_clock::now();

        auto send_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_send_time - start_send_time).count() / 1000.0;
        std::cout << "Image sent\nSuccessfully sent: " << packages_success << "\nFailed to send: " << packages_error << "\nTime: " << send_time << " s." << std::endl;
    }
}

int main()
{
    source = fopen("image.jpg", "rb");
    if (!source)
    {
        std::cerr << "Image not found!" << std::endl;
        return 1;
    }

    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 0);

    if (WSAStartup(wVersionRequested, &wsaData))
    {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET socket_server = socket(
        AF_IPX,
        SOCK_DGRAM,
        NSPROTO_IPX);

    name.sa_family = AF_IPX;

    if (bind(socket_server, (sockaddr *)&name, sizeof(name)))
    {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    int namelen = sizeof(name);
    getsockname(socket_server, (sockaddr *)(&name), &namelen);
    std::cout << "Server is inited" << std::endl;
    printf("Net number: %02hhx %02hhx %02hhx %02hhx\n", name.sa_netnum[0], name.sa_netnum[1], name.sa_netnum[2], name.sa_netnum[3]);
    printf("Node number: %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n", name.sa_nodenum[0], name.sa_nodenum[1], name.sa_nodenum[2], name.sa_nodenum[3], name.sa_nodenum[4], name.sa_nodenum[5]);
    std::cout << "Socket num: " << htons(name.sa_socket) << std::endl;

    bool broadcast = true;
    if (setsockopt(socket_server, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)) == SOCKET_ERROR)
    {
        std::cerr << "Unable to set broadcast" << std::endl;
        closesocket(socket_server);
        WSACleanup();
        return 1;
    }

    send_loop(socket_server);

    if (WSACleanup())
    {
        std::cerr << "WSACleanup failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    closesocket(socket_server);

    std::cout << "Server closed" << std::endl;

    return 0;
}
