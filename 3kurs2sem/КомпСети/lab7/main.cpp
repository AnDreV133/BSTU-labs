#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <schannel.h>
#include <limits>

#define STR_BUF_SIZE 256
#define TEXT_BUF_SIZE 256
#define SMTP_PORT 25
#define POP3_PORT 110

using namespace std;

void throw_error_with_code()
{
    std::string error_msg = "Error with code: ";
    error_msg += to_string(WSAGetLastError());
    throw std::runtime_error(error_msg);
}

const char *getaddrbyname(const char *hostname)
{
    struct hostent *host_info;
    struct in_addr addr = {0};
    char *ip = NULL;

    if ((host_info = gethostbyname(hostname)) == NULL)
        return NULL;

    addr.s_addr = *(u_long *)host_info->h_addr_list[0];

    return inet_ntoa(addr);
}

void startup_wsa();
SOCKET get_socket_descriptor();
sockaddr_in get_bind_addr(const char *address, unsigned short port);
void connect_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr);

SOCKET get_connect(const char *address, unsigned short port)
{
    std::clog << "start connect..." << std::endl;

    startup_wsa();

    std::clog << "WSA started..." << std::endl;

    SOCKET socket_descriptor = get_socket_descriptor();
    std::clog << "create socket" << std::endl;

    sockaddr_in bind_addr = get_bind_addr(address, port);
    std::clog << "create bind address" << std::endl;

    connect_socket_with_address(socket_descriptor, bind_addr);
    std::clog << "connect socket with address\nconnected" << std::endl;

    return socket_descriptor;
}

void startup_wsa()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) == SOCKET_ERROR)
        throw_error_with_code();
}

SOCKET get_socket_descriptor()
{
    SOCKET res = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    if (res == INVALID_SOCKET)
        throw_error_with_code();

    return res;
}

sockaddr_in get_bind_addr(const char *address, unsigned short port)
{
    sockaddr_in res;

    res.sin_family = AF_INET;
    res.sin_addr.s_addr = inet_addr(address);
    res.sin_port = htons(port);

    return res;
}

void connect_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr)
{
    if (connect(socket_descriptor, (sockaddr *)&bind_addr, sizeof(bind_addr)) == SOCKET_ERROR)
        throw_error_with_code();
}

///////////////////////////////////////////////////////

void disconnect(SOCKET connection)
{
    std::clog << "start disconnect..." << std::endl;

    if (closesocket(connection) == SOCKET_ERROR)
        throw_error_with_code();
    WSACleanup();

    std::clog << "disconnected" << std::endl;
}

///////////////////////////////////////////////////////

SOCKET wait_client(SOCKET connection)
{
    sockaddr_in temp;
    int size_temp = sizeof(sockaddr_in);
    SOCKET client = accept(connection, (struct sockaddr *)&temp, &size_temp);

    if (client == INVALID_SOCKET)
        throw_error_with_code();

    return client;
}

///////////////////////////////////////////////////////

struct UserInfo
{
    const char *login;
    const char *password;
};

void send(UserInfo user_info);
void get(UserInfo user_info);

int main(int argc, char *argv[])
{
    UserInfo user_info = {0};
    if (argc == 3)
    {
        user_info.login = argv[1];
        user_info.password = argv[2];
    }
    else
        throw "2 argumnts not wrotten\n";

    int selected_value = 0;
    while (true)
    {
        cout << "Change action:\n"
             << "  1. Send\n"
             << "  2. Get\n"
             << "  3. Exit"
             << endl;
        cin >> selected_value;

        switch (selected_value)
        {
        case 1:
            send(user_info);
            break;
        case 2:
            get(user_info);
            break;
        case 3:
            exit(0);
            break;
        default:
            cout << "Wrong input" << endl;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}

void input_message(string &dst, UserInfo user_info)
{
    string full_message;
    string line;
    while (getline(cin, line) && line != ".")
    {
        full_message += line + "\r\n";
    }

    dst = "From: smtp-client <" + string(user_info.login) + ">\r\n";
    dst += "Subject: smtp-client testmsg\r\n";
    dst += full_message + "\r\n.\r\n";
}

void send(UserInfo user_info)
{
    startup_wsa();

    SOCKET sock = get_connect(getaddrbyname("smtp.freesmtpservers.com"), SMTP_PORT);
    char cmd_buf[STR_BUF_SIZE];
    char text_buf[TEXT_BUF_SIZE];

    // блок конверта
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on start: %s", text_buf);

    sprintf(cmd_buf, "HELO freesmtpservers.com\r\n");
    send(sock, cmd_buf, strlen(cmd_buf), 0);
    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on hello: %s", text_buf);

    sprintf(cmd_buf, "MAIL FROM: <%s>\r\n", user_info.login);
    send(sock, cmd_buf, strlen(cmd_buf), 0);
    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on mail: %s", text_buf);

    cout << "Input recipient: ";
    cin >> text_buf;
    sprintf(cmd_buf, "RCPT TO: <%s>\r\n", text_buf);
    send(sock, cmd_buf, strlen(cmd_buf), 0);
    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on recipient: %s", text_buf);

    send(sock, "DATA\r\n", 6, 0);
    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on data: %s", text_buf);

    // блок сообщения
    cout << "Input message:" << endl;
    string message;
    input_message(message, user_info);
    send(sock, message.c_str(), message.length(), 0);

    printf("\n\n--- msg ---\n\n%s\n\n", message.c_str());

    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on message: %s", text_buf);

    // завершение сессии
    send(sock, "QUIT\r\n", 6, 0);
    memset(text_buf, '\0', TEXT_BUF_SIZE);
    recv(sock, text_buf, TEXT_BUF_SIZE, 0);
    printf("Server on quit: %s", text_buf);

    disconnect(sock);
}

void read_resp(SOCKET sock)
{
    char buffer[TEXT_BUF_SIZE];
    int bytes;
    int total_bytes = 0;

    while ((bytes = recv(sock, buffer + total_bytes, TEXT_BUF_SIZE - total_bytes - 1, 0)) > 0)
    {
        total_bytes += bytes;
        buffer[total_bytes] = '\0';

        // Проверяем завершение ответа (для однострочных команд)
        if (strstr(buffer, "\r\n"))
        {
            printf("%s", buffer);
            break;
        }

        // Проверяем завершение многострочного ответа
        if (strstr(buffer, "\r\n.\r\n"))
        {
            printf("%s", buffer);
            break;
        }

        // Защита от переполнения буфера
        if (total_bytes >= TEXT_BUF_SIZE - 1)
        {
            buffer[TEXT_BUF_SIZE - 1] = '\0';
            printf("%s", buffer);
            break;
        }
    }

    if (bytes <= 0)
    {
        printf("Connection closed or error\n");
    }
}

void send_cmd(SOCKET sock, const char *cmd)
{
    send(sock, cmd, strlen(cmd), 0);
}

void get(UserInfo user_info)
{
    startup_wsa();

    SOCKET sock = get_connect(getaddrbyname("pop.mypop3.com"), POP3_PORT);
    char cmd_buf[STR_BUF_SIZE];
    char input_buf[STR_BUF_SIZE];
    int message_count = 0;

    printf("Server on start: ");
    read_resp(sock);

    // авторизация
    sprintf(cmd_buf, "USER %s\r\n", user_info.login);
    send_cmd(sock, cmd_buf);
    printf("Server on user: ");
    read_resp(sock);
      
    sprintf(cmd_buf, "PASS %s\r\n", user_info.password);
    send_cmd(sock, cmd_buf);
    printf("Server on password: ");
    read_resp(sock);

    // запрос количества писем
    send_cmd(sock, "STAT\r\n");
    printf("Server on stat: ");
    char* stat_response = read_resp(sock);
    if (stat_response != NULL) {
        sscanf(stat_response, "+OK %d", &message_count);
        printf("Total messages: %d\n", message_count);
    }

    // запрос списка писем
    send_cmd(sock, "LIST\r\n");
    printf("Server on list: ");
    read_resp(sock);

    printf("Enter commands ('see N' to view or 'del N' to delete, 'quit' to exit):\n");
    while (1) {
        printf("> ");
        fgets(input_buf, STR_BUF_SIZE, stdin);
        input_buf[strcspn(input_buf, "\n")] = '\0';

        if (strncmp(input_buf, "see ", 4) == 0) {
            int msg_num = atoi(input_buf + 4);
            if (msg_num > 0 && msg_num <= message_count) {
                sprintf(cmd_buf, "RETR %d\r\n", msg_num);
                send_cmd(sock, cmd_buf);
                printf("Server on retr %d: ", msg_num);
                read_resp(sock);
            } else {
                printf("Invalid message number. Available: 1-%d\n", message_count);
            }
        }
        else if (strncmp(input_buf, "del ", 4) == 0) {
            int msg_num = atoi(input_buf + 4);
            if (msg_num > 0 && msg_num <= message_count) {
                sprintf(cmd_buf, "DELE %d\r\n", msg_num);
                send_cmd(sock, cmd_buf);
                printf("Server on dele %d: ", msg_num);
                read_resp(sock);
            } else {
                printf("Invalid message number. Available: 1-%d\n", message_count);
            }
        }
        else if (strcmp(input_buf, "quit") == 0) {
            break;
        }
        else {
            printf("Unknown command. Use 'see N', 'del N' or 'quit'\n");
        }
    }

    // завершение сессии
    send_cmd(sock, "QUIT\r\n");
    printf("Server on quit: ");
    read_resp(sock);

    disconnect(sock);
}

/*
void get(UserInfo user_info)
{
    startup_wsa();

    SOCKET sock = get_connect(getaddrbyname("pop.mypop3.com"), POP3_PORT);
    char cmd_buf[STR_BUF_SIZE];
    char input_buf[STR_BUF_SIZE];
    int message_count = 0;

    printf("Server on start: ");
    read_resp(sock);

    // авторизация
    sprintf(cmd_buf, "USER %s\r\n", user_info.login);
    send_cmd(sock, cmd_buf);
    printf("Server on user: ");
    read_resp(sock);
      
    sprintf(cmd_buf, "PASS %s\r\n", user_info.password);
    send_cmd(sock, cmd_buf);
    printf("Server on password: ");
    read_resp(sock);

    // запрос количества писем
    send_cmd(sock, "STAT\r\n");
    printf("Server on stat: ");
    char* stat_response = read_resp(sock);
    if (stat_response != NULL) {
        sscanf(stat_response, "+OK %d", &message_count);
        printf("Total messages: %d\n", message_count);
    }

    // запрос списка писем
    send_cmd(sock, "LIST\r\n");
    printf("Server on list: ");
    read_resp(sock);

    printf("Enter commands ('see N' to view or 'del N' to delete, 'quit' to exit):\n");
    while (1) {
        printf("> ");
        fgets(input_buf, STR_BUF_SIZE, stdin);
        input_buf[strcspn(input_buf, "\n")] = '\0';

        if (strncmp(input_buf, "see ", 4) == 0) {
            int msg_num = atoi(input_buf + 4);
            if (msg_num > 0 && msg_num <= message_count) {
                sprintf(cmd_buf, "RETR %d\r\n", msg_num);
                send_cmd(sock, cmd_buf);
                printf("Server on retr %d: ", msg_num);
                read_resp(sock);
            } else {
                printf("Invalid message number. Available: 1-%d\n", message_count);
            }
        }
        else if (strncmp(input_buf, "del ", 4) == 0) {
            int msg_num = atoi(input_buf + 4);
            if (msg_num > 0 && msg_num <= message_count) {
                sprintf(cmd_buf, "DELE %d\r\n", msg_num);
                send_cmd(sock, cmd_buf);
                printf("Server on dele %d: ", msg_num);
                read_resp(sock);
            } else {
                printf("Invalid message number. Available: 1-%d\n", message_count);
            }
        }
        else if (strcmp(input_buf, "quit") == 0) {
            break;
        }
        else {
            printf("Unknown command. Use 'see N', 'del N' or 'quit'\n");
        }
    }

    // завершение сессии
    send_cmd(sock, "QUIT\r\n");
    printf("Server on quit: ");
    read_resp(sock);

    disconnect(sock);
}

*/

/*
printf("Server on password: +OK\n");
    printf("Server on list: +OK\n1 303\n2 300\n3 301\n.\n");
    printf("Server on retr: +OK\n");
    printf("From: smtp-client <user@mypop3.com>\n");
    printf("Subject: smtp-client testmsg\n");
    printf("\n\n");
    printf("test test\n");
*/

/*
printf("Server on start: +OK POP3 server ready\n");
    printf("Server on user: +OK\n");
    printf("Server on password: +OK\n");
    printf("Server on stat: +OK 3 904\n");
    printf("Total messages: 3\n");
    printf("Server on list: +OK\n");
    printf("1 303\n");
    printf("2 300\n");
    printf("3 301\n");
    printf(".\n");
    printf("\n");
    printf("Enter commands ('see N' to view or 'del N' to delete, 'quit' to exit):\n");
    printf("> see 1\n");
    printf("Server on retr 1: +OK\n");
    printf("From: smtp-client <user@mypop3.com>\n");
    printf("Subject: smtp-client testmsg\n");
    printf("\n");
    printf("\n");
    printf("test test\n");
    printf("\n");
    printf("> del 1\n");
    printf("Server on dele 1: +OK Message marked for deletion\n");
    printf("> quit\n");
    printf("Server on quit: +OK Goodbye\n");

*/