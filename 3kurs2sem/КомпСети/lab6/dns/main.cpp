#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>

#define STR_BUF_SIZE 1024

using namespace std;

void throw_error_with_code()
{
    std::string error_msg = "Error with code: ";
    error_msg += to_string(WSAGetLastError());
    throw std::runtime_error(error_msg);
}

void view_ip_by_domain()
{
    struct hostent *remote_host;
    struct in_addr addr = {0};
    char host_name[STR_BUF_SIZE];

    cout << "Enter domain: ";
    cin.ignore();
    cin.getline(host_name, STR_BUF_SIZE);

    remote_host = gethostbyname(host_name);
    if (remote_host == NULL)
    {
        cerr << "Remote host not defined";
        return;
    }

    if (WSAGetLastError())
        throw_error_with_code();

    if (remote_host->h_addrtype == AF_INET)
    {
        int i = 0;
        while (remote_host->h_addr_list[i] != 0)
        {
            addr.s_addr = *(u_long *)remote_host->h_addr_list[i++];
            printf("\tIP address #%d: %s\n", i, inet_ntoa(addr));
        }
    }
    cout << endl;
}

void view_domain_by_ip()
{
    struct hostent *remote_host;

    cout << "Enter IP address: ";
    char ip_addr[STR_BUF_SIZE];
    cin.ignore();
    cin.getline(ip_addr, STR_BUF_SIZE);

    DWORD ip = inet_addr(ip_addr);
    remote_host = gethostbyaddr((char *)&ip, 4, AF_INET);
    cout << "\tDomain name for IP address: "
         << remote_host->h_name
         << '\n'
         << endl;
}

int main()
{
    while (true)
    {
        WSADATA wsa_data;
        int result;
        struct hostent *remote_host;
        struct in_addr addr = {0};

        result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (result != 0)
            throw "WSAStartup failed: %d\n", result;

        cout << "1. Find IP address by domain name\n"
             << "2. Find domain name by IP address\n"
             << "3. Exit\n"
             << "Choice: "
             << endl;
        int selected_value;
        cin >> selected_value;

        switch (selected_value)
        {
        case 1:
            view_ip_by_domain();
            break;
        case 2:
            view_domain_by_ip();
            break;
        case 3:
        default:
            return 0;
        }
    }
}