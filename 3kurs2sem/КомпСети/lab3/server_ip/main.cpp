#include <WinSock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <iostream>
#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <pthread.h>

#define FILE_FRAGMENT_SIZE 512

bool should_run = false;

struct server_args
{
    SOCKET server_socket;
    sockaddr_in client_sockaddr;
};

void throw_err_with_code()
{
    std::string err_msg = "error with code: ";
    err_msg += std::to_string(WSAGetLastError());
    throw std::runtime_error(err_msg);
}

void startup_wsa();
SOCKET get_socket_descriptor();
void set_option_timeout(SOCKET socket_descriptor);
sockaddr_in get_bind_addr(const char *address, unsigned short port);
void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr);

SOCKET connect(const char *address, unsigned short port)
{
    std::clog << "start connect..." << std::endl;

    startup_wsa();

    std::clog << "WSA started..." << std::endl;

    SOCKET socket_descriptor = get_socket_descriptor();
    std::clog << "create socket" << std::endl;

    set_option_timeout(socket_descriptor);
    std::clog << "set option: broadcast" << std::endl;

    sockaddr_in bind_addr = get_bind_addr(address, port);
    std::clog << "create bind address" << std::endl;

    bind_socket_with_address(socket_descriptor, bind_addr);
    std::clog << "bind socket with address\nconnected" << std::endl;

    return socket_descriptor;
}

void startup_wsa()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 0);

    if (WSAStartup(wVersionRequested, &wsaData) == SOCKET_ERROR)
        throw_err_with_code();
}

SOCKET get_socket_descriptor()
{
    SOCKET res = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_IP);

    if (res == INVALID_SOCKET)
        throw_err_with_code();

    return res;
}

void set_option_timeout(SOCKET socket_descriptor)
{
    bool broadcast = true;
    if (
        setsockopt(
            socket_descriptor,
            SOL_SOCKET,
            SO_BROADCAST,
            (char *)&broadcast,
            sizeof(broadcast)) == SOCKET_ERROR)
        throw_err_with_code();
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
        throw_err_with_code();
}

///////////////////////////////////////////////////////////////////////////////////////////

void disconnect(SOCKET connection)
{
    std::clog << "start disconnect..." << std::endl;

    if (closesocket(connection) == SOCKET_ERROR)
        throw_err_with_code();
    WSACleanup();

    std::clog << "disconnected" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////

void send_file_with_input(SOCKET con, sockaddr_in client_addr);
void send_file_by_default(SOCKET con, sockaddr_in client_addr);

void *start_loop_server(void *args)
{
    struct server_args typed_args = *((server_args *)args);

    SOCKET con = typed_args.server_socket;
    sockaddr_in client_addr = typed_args.client_sockaddr;

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
            send_file_with_input(con, client_addr);
            break;

        case 'd':
        case 'D':
            send_file_by_default(con, client_addr);
            break;

        case 'c':
        case 'C':
            should_run = false;
            break;

        default:
            std::cout << "incorrect input, try again" << std::endl;
            break;
        }
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

void send_file_by_path(SOCKET con, sockaddr_in client_addr, std::string file_path);

void send_file_with_input(SOCKET con, sockaddr_in client_addr)
{
    std::cout << "input path to file from current dir:" << std::endl;
    std::string file_path;
    std::cin >> file_path;

    send_file_by_path(con, client_addr, file_path);
}

void send_file_by_default(SOCKET con, sockaddr_in client_addr)
{
    send_file_by_path(con, client_addr, std::string("./image.jpg"));
}

std::ifstream *new_get_file(std::string path);
std::ifstream get_file(std::string path);
void send_file(SOCKET con, sockaddr_in client_addr, std::ifstream *file);

void send_file_by_path(SOCKET con, sockaddr_in client_addr, std::string file_path)
{
    std::clog << "start send file..." << std::endl;

    std::ifstream *file = new_get_file(file_path);

    std::clog << "file opened..." << std::endl;

    send_file(con, client_addr, file);

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

void send_file(SOCKET con, sockaddr_in client_addr, std::ifstream *file)
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

        if (sendto(
                con,
                buffer,
                bytes_read,
                0,
                (sockaddr *)&client_addr,
                sizeof(client_addr)) == SOCKET_ERROR)
        {
            packages_failed++;
        }
        else
        {
            packages_success++;
        }
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
    auto con = connect("192.168.1.215", 0x8080);
    auto client_addr = get_bind_addr("192.168.1.215", 0x8081);

    struct server_args args = {con, client_addr};
    auto server_thread = run_server(args);

    pthread_join(server_thread, NULL);

    disconnect(con);

    return 0;
}