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
#include <random>

#define FILE_FRAGMENT_SIZE 512

bool should_run = false;

void throw_err_with_code()
{
    std::string err_msg = "error with code: ";
    err_msg += std::to_string(WSAGetLastError());
    throw std::runtime_error(err_msg);
}

void startup_wsa();
SOCKET get_socket_descriptor();
void set_option_timeout(SOCKET socket_descriptor, unsigned int timeout_ms);
sockaddr_in get_bind_addr(const char *address, unsigned short port);
void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr);

SOCKET connect(const char *address, unsigned short port)
{
    std::clog << "start connect..." << std::endl;

    startup_wsa();

    std::clog << "WSA started..." << std::endl;

    SOCKET socket_descriptor = get_socket_descriptor();
    std::clog << "create socket" << std::endl;

    set_option_timeout(socket_descriptor, 10000);
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

void set_option_timeout(SOCKET socket_descriptor, unsigned int timeout_ms)
{
    if (
        setsockopt(
            socket_descriptor,
            SOL_SOCKET,
            SO_RCVTIMEO,
            (char *)&timeout_ms,
            sizeof(timeout_ms)) == SOCKET_ERROR)
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

void recv_file(SOCKET con);

void handle_client(SOCKET con)
{
    should_run = true;

    recv_file(con);
}

///////////////////////////////////////////////////////////////////////////////////////////

std::string generate_filename();
std::ofstream create_output_file(std::string filename);
void save_file_fragment(std::ofstream &file, const char *data, size_t size);

void recv_file(SOCKET con)
{
    char buffer[FILE_FRAGMENT_SIZE];
    std::string filename = generate_filename();
    std::ofstream out_file = create_output_file(filename);
    int bytes_received;

    auto a = std::chrono::high_resolution_clock::now();
    while (should_run && (bytes_received = recvfrom(
                              con,
                              buffer,
                              sizeof(buffer),
                              0,
                              nullptr,
                              nullptr)) != SOCKET_ERROR)
        save_file_fragment(out_file, buffer, bytes_received);
    auto b = std::chrono::high_resolution_clock::now();

    std::clog << "Answer accepted\n"
              << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count() / 1000.0 << " s." << std::endl;
}

std::string generate_filename()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return "file_" + std::to_string(dis(gen)) + ".jpg";
}

std::ofstream create_output_file(std::string filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to create file: " + filename);
    }
    return file;
}

void save_file_fragment(std::ofstream &file, const char *data, size_t size)
{
    file.write(data, size);
    if (!file.good())
    {
        throw std::runtime_error("File write error");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto con = connect("192.168.1.215", 0x8081);

    handle_client(con);

    disconnect(con);

    return 0;
}