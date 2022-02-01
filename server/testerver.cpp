#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
#include <sstream>
#include <fstream>

#define PORT 8080
#define BACKLOG 100

void	error_check(int err, std::string msg)
{
	if (err < 0)
	{
		std::cout << "Error: " << msg << std::endl;
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in get_addr()
{
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	return address;
}

void	handle_connection(int cli_sock)
{
	char buffer[1024] = {0};
	std::string file_path;
	int bytes_read = 0;

	/* Parsing the request */
	read(cli_sock, buffer, 1024);
	std::cout << std::string(buffer) << std::endl;
	file_path = std::string(buffer).replace(0, std::string("REQUEST: ").length(), "");

	/* Checking/opening the requested file */
	FILE *file = fopen(file_path.c_str(), "r");
	if (file == NULL)
	{
		std::cout << "Couldnt open " << file_path << std::endl << "Closed connection" << std::endl;
		close(cli_sock);
		return ;
	}

	/* Reading from file and writing to cli */
	while ((bytes_read = fread(buffer, 1, 1024, file)) > 0)
	{
		std::cout << "Sending " << bytes_read << " bytes" << std::endl;
		send(cli_sock , buffer, bytes_read, 0);
	}

	/* closing socket, later it whould be kept alive too */
	fclose(file);
	close(cli_sock);
	std::cout << "Closed connection" << std::endl;
}

int main()
{
	int server_sock, cli_sock;
	struct sockaddr_in address = get_addr();
	int opted = 1;
	int address_len = sizeof(address);
	struct pollfd fds;
	int flags;

	/* Getting a socket with ip4 protocol and socket stream */
	error_check(server_sock = socket(AF_INET, SOCK_STREAM, 0), "getting the server socket");
	/* Getting socket flags with fcntl */
	error_check(flags = fcntl(server_sock, F_GETFL), "getting the server socket flags");
	/* Setting the non-blocking flag */
	error_check(fcntl(server_sock, F_SETFL, flags | O_NONBLOCK), "setting the server socket as non-blocking");
	/* Initializing the socket on socket level using the reuseaddr protocol */
	error_check(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)), "initializing the server socket");
	/* Binding the socket */
	error_check(bind(server_sock, (struct sockaddr *)&address, sizeof(address)), "binding the server socket");
	/* Start listening and allow a backlog of 100*/
	error_check(listen(server_sock, BACKLOG), "listening on the server socket");
	while (true)
	{
		std::cout << "Waiting for connections..." << std::endl;
		if ((cli_sock = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
		{
			if (errno == EWOULDBLOCK)
				sleep(1);
			else
				error_check(-1, "Accepting a connection");
		}
		else
		{
			// poll <- add acepted client
			std::cout << "Connected!" << std::endl;
			handle_connection(cli_sock); // <- fd array handelen
		}
			
	}
	return 0;
}