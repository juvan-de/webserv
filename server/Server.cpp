#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>

#define PORT 8080

// char	*server_thonking()
// {
// 	sleep(1);
// 	return ("response m8\n");
// }

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

int main()
{
	int server_sock, cli_sock, reader;
	struct sockaddr_in address = get_addr();
	int opted = 1;
	int address_len = sizeof(address);
	char buffer[1024] = {0};
	std::string message = "A message from server !";
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
	/* Start listening */
	error_check(listen(server_sock, 3), "listening on the server socket");
	std::cout << "Listening to input" << std::endl;
	while (1)
	{
		// cli_sock = accept(server, NULL, NULL);
		if ((cli_sock = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
		{
			if (errno == EWOULDBLOCK)
			{
				//printf("No pending connections; sleeping for one second.\n");
				sleep(1);
			}
			else
				error_check(-1, "Accepting a connection");
		}
		else
		{
			reader = read(cli_sock, buffer, 1024);
			printf("%s\n", buffer);
		}
	}
	// reader = read(cli_sock, buffer, 1024);
	// printf("%s\n", buffer);
	send(cli_sock , message.c_str(), message.length() , 0 );
	printf("Server : Message has been sent ! \n");
	return 0;
}