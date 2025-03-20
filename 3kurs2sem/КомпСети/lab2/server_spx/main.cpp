#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <winsock2.h>
#include <wsipx.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <chrono>
#include <pthread.h>
#include <thread>

#define CLIENT_REQUEST_SIZE 512
#define IMAGE_PART_SIZE 400
#define BACKLOG_MAX_SIZE 10
#define IPX_SOCKET (0x8060)
#define SPX_SOCKET (0x5E86)
#define CHECK_MSG_QUESTION "What are u say about my momo`s sports steering wheel?"
#define CHECK_MSG_ANSWER "I`dont what u... uh, just go"

bool should_run = true;

void* ping_message_task(void* arg)
{
    SOCKADDR_IPX name = {};
    SOCKET socket_descriptor = socket(
        AF_IPX,
        SOCK_DGRAM,
        NSPROTO_IPX);
    name.sa_family = AF_IPX;
    name.sa_socket = htons(IPX_SOCKET);

    if (bind(socket_descriptor, (sockaddr *)&name, sizeof(name)))
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        return nullptr;
    }

    int namelen = sizeof(name);
    getsockname(socket_descriptor, (sockaddr *)(&name), &namelen);
    std::cout << "Ping server is inited\n";
    int timeout_time = 1000;
    setsockopt(
        socket_descriptor,
        SOL_SOCKET,
        SO_RCVTIMEO,
        (char *)&timeout_time,
        sizeof(timeout_time));

    char *accept_client_data = (char *)malloc(sizeof(char) * CLIENT_REQUEST_SIZE);

    while (should_run)
    {
        SOCKADDR_IPX client_sockaddr = {};
        client_sockaddr.sa_family = AF_IPX;
        int client_sockaddr_size = sizeof(client_sockaddr);

        if (recvfrom(
                socket_descriptor,
                accept_client_data,
                sizeof(char) * CLIENT_REQUEST_SIZE,
                0,
                (sockaddr *)&client_sockaddr,
                &client_sockaddr_size) == SOCKET_ERROR)
        {
        }
        else if (strcmp(accept_client_data, CHECK_MSG_QUESTION) == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            char answer[] = CHECK_MSG_ANSWER;
            sendto(socket_descriptor,
                   answer,
                   sizeof(answer),
                   0,
                   (sockaddr *)&client_sockaddr,
                   client_sockaddr_size);
        }
    }

    closesocket(socket_descriptor);
    free(accept_client_data);
    return nullptr;
}

void* serve_client_task(void* arg)
{
    SOCKET client_socket = (SOCKET) arg;

    printf("Serving a new request\n");
    char client_buffer[CLIENT_REQUEST_SIZE] = {};
    if (recv(client_socket, client_buffer, sizeof(client_buffer), 0) == SOCKET_ERROR)
    {
        printf("Failed to get message for client: %d\n", WSAGetLastError());
        closesocket(client_socket);
        return nullptr;
    }

    printf("A request for file '%s'\n", client_buffer);

    FILE *source = fopen(client_buffer, "rb");
    if (!source)
    {
        std::cerr << "File not found" << std::endl;
        closesocket(client_socket);
        return nullptr;
    }

    std::cout << "Everything is OK, sending a file..." << std::endl;

    auto start_send_time = std::chrono::high_resolution_clock::now();
    int bytes_read = 0;
    char image_buffer[IMAGE_PART_SIZE];
    while ((bytes_read = fread(image_buffer, sizeof(char), IMAGE_PART_SIZE, source)))
    {
        if (send(client_socket,
                 image_buffer,
                 bytes_read,
                 0) == SOCKET_ERROR)
        {
            std::cerr << "Failed to send package: %d" << WSAGetLastError() << std::endl;
            closesocket(client_socket);
            fclose(source);
            return nullptr;
        }
    }

    auto end_send_time = std::chrono::high_resolution_clock::now();

    auto send_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_send_time - start_send_time).count() / 1000.0;
    std::cout << "File sent successfully\nTime:" << send_time << " s." << std::endl;

    closesocket(client_socket);
    fclose(source);
    return nullptr;
}

void* mainloop_task(void* arg)
{
    SOCKADDR_IPX name = {};
    SOCKET socket_descriptor = socket(
                                    AF_IPX,
                                    SOCK_SEQPACKET,
                                    NSPROTO_SPX
                                    );
    name.sa_family = AF_IPX;
    name.sa_socket = htons(SPX_SOCKET);

    if (bind(socket_descriptor, (sockaddr*)&name, sizeof(name)) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
    }

    int namelen = sizeof(name);
    getsockname(socket_descriptor, (sockaddr *)(&name), &namelen);
    std::cout << "Server is inited\n";
    printf("Net number: %02hhx %02hhx %02hhx %02hhx\n", name.sa_netnum[0], name.sa_netnum[1], name.sa_netnum[2], name.sa_netnum[3]);
    printf("Node number: %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n", name.sa_nodenum[0], name.sa_nodenum[1], name.sa_nodenum[2], name.sa_nodenum[3], name.sa_nodenum[4], name.sa_nodenum[5]);
    std::cout << "Socket num: " << htons(name.sa_socket) << "\n";
    std::cout.flush();

    if (listen(socket_descriptor, BACKLOG_MAX_SIZE) == SOCKET_ERROR)
    {
        printf("Couldn't startup server (listen failed): %d\n", WSAGetLastError());
        return nullptr;
    }

    while (should_run)
    {
        std::cout << "Awaiting for client connections..." << std::endl;
        SOCKADDR_IPX clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        SOCKET client_descriptor = accept(
            socket_descriptor,
            (sockaddr *)&clientAddr,
            &clientAddrSize);

        if (client_descriptor != INVALID_SOCKET)
        {
            pthread_t serve_client_thread;
            pthread_create(&serve_client_thread, nullptr, serve_client_task, (void*) client_descriptor);
            pthread_detach(serve_client_thread);
        }
    }

    closesocket(socket_descriptor);
    return nullptr;
}

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData))
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    pthread_t ping_thread, mainloop_thread;
    pthread_create(&ping_thread, nullptr, ping_message_task, nullptr);
    pthread_create(&mainloop_thread, nullptr, mainloop_task, nullptr);

    getchar();

    should_run = false;
    std::cout << "Server close! Quitting threads..." << std::endl;
    pthread_join(ping_thread, nullptr);
    pthread_join(mainloop_thread, nullptr);

    WSACleanup();

    return 0;
}
