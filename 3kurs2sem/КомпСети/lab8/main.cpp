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
#include <sstream>
// #include <pthread.h>

#define STR_BUF_SIZE 256

struct server_args
{
	SOCKET server_socket;
	sockaddr_in socket_address;
};

struct Response
{
	int code;
	std::string head;
	std::string body;
};

std::string stuck_response(Response r)
{
	return r.head + "\r\n\r\n" + r.body;
}

Response get_server_error_page()
{
	int code = 501;
	std::stringstream responseHead;
	std::stringstream responseBody;

	responseBody << "<!DOCTYPE HTML>"
				 << "<html>"
				 << "<head>"
				 << "<title>" << std::to_string(code) << " Not Implemented</title>"
				 << "</head>"
				 << "<body>"
				 << "<h1>Not Implemented</h1>"
				 << "<p>This request can not be processed by this server</p>"
				 << "</body>"
				 << "</html>";

	responseHead << "HTTP/1.1 " << std::to_string(code) << " Not Implemented\r\n"
				 << "Version: HTTP/1.1\r\n"
				 << "Content-Type: text/html; charset=utf-8\r\n"
				 << "Content-Length: " << responseBody.str().length();

	return {
		code,
		responseHead.str(),
		responseBody.str()};
}

Response get_ok_page_headers()
{
	int code = 200;
	std::stringstream responseHead;
	std::stringstream responseBody;

	responseHead << "HTTP/1.1 " << std::to_string(code) << " OK\r\n"
				 << "Version: HTTP/1.1\r\n"
				 << "Content-Type: text/html; charset=utf-8\r\n"
				 << "Content-Length: "
				 << responseBody.str().length()
				 << "\r\n\r\n";

	return {
		code,
		responseHead.str(),
		responseBody.str()};
}

Response get_ok_page()
{
	int code = 200;
	std::stringstream responseHead;
	std::stringstream responseBody;

	responseBody << "<!DOCTYPE HTML>"
				 << "<html>"
				 << "<head>"
				 << "<title>" << std::to_string(code) << " Good</title>"
				 << "</head>"
				 << "<body>"
				 << "<h1>Hello world!</h1>"
				 << "<p>Are you want see another?</p>"
				 << "</body>"
				 << "</html>";

	responseHead << "HTTP/1.1 " << std::to_string(code) << " OK\r\n"
				 << "Version: HTTP/1.1\r\n"
				 << "Content-Type: text/html; charset=utf-8\r\n"
				 << "Content-Length: "
				 << responseBody.str().length()
				 << "\r\n\r\n";

	return {
		code,
		responseHead.str(),
		responseBody.str()};
}

Response get_client_error_page()
{
	int code = 404;
	std::stringstream responseHead;
	std::stringstream responseBody;

	responseBody << "<!DOCTYPE HTML>"
				 << "<html>"
				 << "<head>"
				 << "<title>404 Not Found</title>"
				 << "</head>"
				 << "<body>"
				 << "<h1> 404 </h1>"
				 << "<p></p>"
				 << "</body>"
				 << "</html>";

	responseHead << "HTTP/1.1 " << std::to_string(code) << " Not Found\r\n"
				 << "Version: HTTP/1.1\r\n"
				 << "Content-Type: text/html; charset=utf-8\r\n"
				 << "Content-Length: "
				 << responseBody.str().length()
				 << "\r\n\r\n";

	return {
		code,
		responseHead.str(),
		responseBody.str()};
}

bool should_run = false;
std::vector<SOCKET> clients;

void throw_error_with_code()
{
	std::string err_msg = "error with code: ";
	err_msg += std::to_string(WSAGetLastError());
	throw std::runtime_error(err_msg);
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
void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr);
void listen_connections(SOCKET socket_descriptor);

SOCKET connect(const char *address, unsigned short port)
{
	std::clog << "start connect..." << std::endl;

	startup_wsa();

	std::clog << "WSA started..." << std::endl;

	SOCKET socket_descriptor = get_socket_descriptor();
	std::clog << "create socket" << std::endl;

	sockaddr_in bind_addr = get_bind_addr(address, port);
	std::clog << "create bind address" << std::endl;

	bind_socket_with_address(socket_descriptor, bind_addr);
	std::clog << "bind socket with address\nconnected" << std::endl;

	listen_connections(socket_descriptor);
	std::clog << "listen started" << std::endl;

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

void bind_socket_with_address(SOCKET socket_descriptor, sockaddr_in bind_addr)
{
	if (bind(socket_descriptor, (sockaddr *)&bind_addr, sizeof(bind_addr)) == SOCKET_ERROR)
		throw_error_with_code();
}

void listen_connections(SOCKET socket_descriptor)
{
	if (listen(socket_descriptor, 3) == SOCKET_ERROR)
		throw_error_with_code();
}

void handle_client(SOCKET client_socket);

void loop_accept_clients(SOCKET con)
{
	should_run = true;
	while (should_run)
	{
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);
		std::cout << "waiting accept" << std::endl;
		SOCKET client_socket = accept(con, (sockaddr *)&client_addr, &client_addr_size);

		if (client_socket != INVALID_SOCKET)
		{
			std::cout << "handle client" << std::endl;
			handle_client(client_socket);
		}
		else
			throw_error_with_code();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

void handle_client(SOCKET client_socket)
{
	char request_buffer[STR_BUF_SIZE];
	int request_buffer_len = STR_BUF_SIZE;
	int iResult;
	int iSendResult;
	char method[STR_BUF_SIZE];
	char URI[STR_BUF_SIZE];
	char host[STR_BUF_SIZE];
	int ver_h = 0;
	int ver_l = 0;

	iResult = recv(client_socket, request_buffer, request_buffer_len, 0);
	if (iResult > 0)
	{
		request_buffer[iResult] = '\0';
		sscanf(request_buffer, "%s %s HTTP/%i.%i\nHost: %s", method, URI, &ver_h, &ver_l, host);
		printf("Client ask method %s for URI %s (HTTP ver %i.%i)\nHost: %s\n", method, URI, ver_h, ver_l, host);
	}
	else if (iResult == 0)
	{
		printf("Close connection\n");
		return;
	}
	else
		throw_error_with_code();

	Response response = {};
	if (strcmp(URI, "/index.html"))
		response = get_client_error_page();
	else if (strcmp(method, "GET") == 0)
		response = get_ok_page();
	else if (strcmp(method, "HEAD") == 0)
		response = get_ok_page_headers();
	else
		response = get_server_error_page();

	std::string stucked_response = stuck_response(response);
	iSendResult = send(
		client_socket,
		stucked_response.c_str(),
		stucked_response.length(),
		0);
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

int main()
{
	auto con = connect("127.0.0.1", 80);

	std::clog << "start accept clients" << std::endl;
	loop_accept_clients(con);

	disconnect(con);

	return 0;
}