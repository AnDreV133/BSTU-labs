#include <WinSock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <iostream>
#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <pthread.h>

#define FILE_FRAGMENT_SIZE 512

enum TransportProtocol
{
    TCP,
    UDP
};

struct server_args
{
    SOCKET server_socket;
    sockaddr_in socket_address;
    TransportProtocol protocol;
};

bool should_run = false;
std::vector<SOCKET> clients;

void throw_error_with_code()
{
    std::string err_msg = "error with code: ";
    err_msg += std::to_string(WSAGetLastError());
    throw std::runtime_error(err_msg);
}

void startup_wsa();
SOCKET get_socket_descriptor_tcp();
SOCKET get_socket_descriptor_udp();
void set_option_broadcast(SOCKET socket_descriptor);
sockaddr_in get_bind_addr(const char *address, unsigned short port);
void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr);
void listen_connections(SOCKET socket_descriptor);
pthread_t run_accept_clients(SOCKET connection);

SOCKET connect(const char *address, unsigned short port, TransportProtocol protocol)
{
    std::clog << "start connect..." << std::endl;

    startup_wsa();

    std::clog << "WSA started..." << std::endl;

    SOCKET socket_descriptor;
    if (protocol == TCP)
        socket_descriptor = get_socket_descriptor_tcp();
    else if (protocol == UDP)
        socket_descriptor = get_socket_descriptor_udp();
    std::clog << "create socket" << std::endl;

    // if (protocol == UDP)
    // {
    //     set_option_broadcast(socket_descriptor);
    //     std::clog << "set option: broadcast" << std::endl;
    // }

    sockaddr_in bind_addr = get_bind_addr(address, port);
    std::clog << "create bind address" << std::endl;

    bind_socket_with_address(socket_descriptor, bind_addr);
    std::clog << "bind socket with address\nconnected" << std::endl;

    if (protocol == TCP)
    {
        listen_connections(socket_descriptor);
        std::clog << "listen started" << std::endl;

        std::clog << "start accept clients" << std::endl;
        run_accept_clients(socket_descriptor);
    }

    return socket_descriptor;
}

void startup_wsa()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 0);

    if (WSAStartup(wVersionRequested, &wsaData) == SOCKET_ERROR)
        throw_error_with_code();
}

SOCKET get_socket_descriptor_tcp()
{
    SOCKET res = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (res == INVALID_SOCKET)
        throw_error_with_code();

    return res;
}

SOCKET get_socket_descriptor_udp()
{
    SOCKET res = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_IP);

    if (res == INVALID_SOCKET)
        throw_error_with_code();

    return res;
}

void set_option_broadcast(SOCKET socket_descriptor)
{
    bool broadcast = true;
    if (
        setsockopt(
            socket_descriptor,
            SOL_SOCKET,
            SO_BROADCAST,
            (char *)&broadcast,
            sizeof(broadcast)) == SOCKET_ERROR)
        throw_error_with_code();
}

sockaddr_in get_bind_addr(const char *address, unsigned short port)
{
    sockaddr_in res;

    res.sin_family = AF_INET;
    res.sin_addr.s_addr = inet_addr(address);
    res.sin_port = htons(port);

    return res;
}

void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr)
{
    if (bind(socket_descriptor, (sockaddr *)&bind_addr, sizeof(bind_addr)) == SOCKET_ERROR)
        throw_error_with_code();
}

void listen_connections(SOCKET socket_descriptor)
{
    if (listen(socket_descriptor, SOMAXCONN) == SOCKET_ERROR)
        throw_error_with_code();
}

void *start_loop_accept_clients(void *arg)
{
    SOCKET con = ((SOCKET)(intptr_t)arg);
    
    should_run = true;
    while (should_run)
    {
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(con, (sockaddr *)&client_addr, &client_addr_size);

        clients.emplace_back(client_socket);

        if (client_socket != INVALID_SOCKET)
            std::clog << "client accepted" << std::endl;
        else
            throw_error_with_code();
            // std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
    }

    return nullptr;
}

pthread_t run_accept_clients(SOCKET connection)
{
    pthread_t thread;
    if (pthread_create(&thread, nullptr, &start_loop_accept_clients, (void *)(intptr_t)connection) != 0)
    {
        throw "error when start thread\n";
    }

    return thread;
}

///////////////////////////////////////////////////////////////////////////////////////////

void disconnect(SOCKET connection)
{
    std::clog << "start disconnect..." << std::endl;

    if (closesocket(connection) == SOCKET_ERROR)
        throw_error_with_code();
    WSACleanup();

    std::clog << "disconnected" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////

void send_file_by_path(SOCKET con, sockaddr_in client_addr, std::string file_path, TransportProtocol protocol);

void *start_loop_server(void *args)
{
    struct server_args typed_args = *((server_args *)args);

    SOCKET con = typed_args.server_socket;
    TransportProtocol protocol = typed_args.protocol;

    should_run = true;
    while (should_run)
    {
        std::cout << "input operation key:" << std::endl;
        char operation_key = '\0';
        std::cin >> operation_key;

        switch (operation_key)
        {
        case 'i':
        case 'I':
        {
            std::cout << "input path to file from current dir:" << std::endl;
            std::string file_path;
            std::cin >> file_path;

            if (protocol == UDP)
                send_file_by_path(con, typed_args.socket_address, file_path, protocol);
            else if (protocol == TCP)
                for (SOCKET &client : clients)
                    send_file_by_path(client, {}, file_path, protocol);
        }
        break;

        case 'd':
        case 'D':
            if (protocol == UDP)
                send_file_by_path(con, typed_args.socket_address, std::string("./image.jpg"), protocol);
            else if (protocol == TCP)
                for (SOCKET &client : clients)
                    send_file_by_path(client, {}, std::string("./image.jpg"), protocol);

            break;

        case 'c':
        case 'C':
            should_run = false;
            break;

        default:
            std::cout << "incorrect input, try again" << std::endl;
            break;
        }

        for (SOCKET &sock : clients) {
            closesocket(sock);
        }

        clients.clear();
    }

    return nullptr;
}

pthread_t run_server(struct server_args connection)
{
    pthread_t thread;
    if (pthread_create(&thread, nullptr, &start_loop_server, (void *)&connection) != 0)
    {
        throw "error when start thread\n";
    }

    return thread;
}

///////////////////////////////////////////////////////////////////////////////////////////

std::ifstream *new_get_file(std::string path);
std::ifstream get_file(std::string path);
void send_file(SOCKET con, sockaddr_in client_addr, std::ifstream *file, TransportProtocol protocol);

void send_file_by_path(SOCKET con, sockaddr_in client_addr, std::string file_path, TransportProtocol protocol)
{
    std::clog << "start send file..." << std::endl;

    std::ifstream *file = new_get_file(file_path);

    std::clog << "file opened..." << std::endl;

    send_file(con, client_addr, file, protocol);

    file->close();
    delete file;
}

std::ifstream *new_get_file(std::string path)
{
    std::ifstream *file = new std::ifstream(path, std::ios::binary);
    if (!file->is_open())
        throw "Unable to open file for read: " + path;

    return file;
}

std::ifstream get_file(std::string path)
{
    std::ifstream file = std::ifstream(path, std::ios::binary);
    if (!file.is_open())
        throw "Unable to open file for read: " + path;

    return file;
}

void send_file(SOCKET con, sockaddr_in client_addr, std::ifstream *file, TransportProtocol protocol)
{
    char buffer[FILE_FRAGMENT_SIZE];
    int packages_success = 0, packages_failed = 0;
    int total_bytes = 0;

    auto a = std::chrono::high_resolution_clock::now();
    while (should_run && !file->eof())
    {
        file->read(buffer, sizeof(buffer));
        int bytes_read = file->gcount();
        total_bytes += bytes_read;
        int total_bytes = 0;

        if (protocol == UDP && sendto(
                                   con,
                                   buffer,
                                   bytes_read,
                                   0,
                                   (sockaddr *)&client_addr,
                                   sizeof(client_addr)) != SOCKET_ERROR)
            packages_success++;
        else if (protocol == TCP && send(
                                        con,
                                        buffer,
                                        bytes_read,
                                        0) != SOCKET_ERROR)
            packages_success++;
        else
            packages_failed++;
    }
    auto b = std::chrono::high_resolution_clock::now();

    std::clog << "A file broadcasted ended\n  Packages failed: "
              << packages_failed << "\n  Packages sent: " << packages_success << "\n"
              << "  Size:" << "\n"
              << "    " << total_bytes << " B\n"
              << "    " << total_bytes / 1024.0 << " KiB\n"
              << "    " << total_bytes / 1024.0 / 1024.0 << " MiB\n"
              << "\n  Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count() / 1000.0 << " s."
              << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto con = connect("192.168.1.215", 0x8080, TCP);
    sockaddr_in client_addr; //= get_bind_addr("127.0.0.1", 0x8080);

    struct server_args args = {con, client_addr, TCP};
    auto server_thread = run_server(args);

    pthread_join(server_thread, NULL);

    disconnect(con);

    return 0;
}