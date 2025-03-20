#define WIN32_LEAN_AND_MEAN

#include <time.h>
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <wsipx.h>
#include <stdlib.h>
#include <conio.h>

#define IPX_SOCKET (0x8060)
#define CLIENT_REQUEST_SIZE 512
#define IMAGE_PART_SIZE 400

SOCKADDR_IPX name = {};
SOCKADDR_IPX server_sockaddr = {};

void record_loop(SOCKET socket_descriptor, FILE *dist)
{
    std::cout << "Starting file accept..." << std::endl;
    char *buffer = (char *)malloc(sizeof(char) * IMAGE_PART_SIZE);
    int bytes_received;
    while ((bytes_received = recvfrom(socket_descriptor, buffer, sizeof(char) * IMAGE_PART_SIZE, 0, nullptr, nullptr)) != SOCKET_ERROR)
    {
        fwrite(buffer, sizeof(char), bytes_received, dist);
    }

    free(buffer);
}

int main()
{
    char filename[20];

    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 0);

    if (WSAStartup(wVersionRequested, &wsaData))
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    SOCKET socket_descriptor = socket(
        AF_IPX,
        SOCK_DGRAM,
        NSPROTO_IPX);
    name.sa_family = AF_IPX;
    name.sa_socket = htons(IPX_SOCKET);

    if (bind(socket_descriptor, (sockaddr *)&name, sizeof(name)))
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    int namelen = sizeof(name);
    getsockname(socket_descriptor, (sockaddr *)(&name), &namelen);

    std::cout << "Client is inited:\n";
    printf("Net number: %02hhx %02hhx %02hhx %02hhx\n", name.sa_netnum[0], name.sa_netnum[1], name.sa_netnum[2], name.sa_netnum[3]);
    printf("Node number: %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n", name.sa_nodenum[0], name.sa_nodenum[1], name.sa_nodenum[2], name.sa_nodenum[3], name.sa_nodenum[4], name.sa_nodenum[5]);
    std::cout << "Socket num: " << htons(name.sa_socket) << std::endl;

    int timeout_time = 15000;

    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_time, sizeof(timeout_time)) == SOCKET_ERROR)
    {
        printf("Unable to set timeout: %d\n", WSAGetLastError());
        return 1;
    }

    srand(time(NULL));
    sprintf(filename, "img%d.jpg", rand() % 100 + 1);
    FILE *dist = fopen(filename, "wb");

    record_loop(socket_descriptor, dist);

    fflush(dist);
    fclose(dist);

    if (WSACleanup())
    {
        printf("WSACleanup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    closesocket(socket_descriptor);

    std::cout << "A file was saved at " << filename << "\nPress any button to exit" << std::endl;
    getchar();

    std::cout << "Client close" << std::endl;

    return 0;
}
