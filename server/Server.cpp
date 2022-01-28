#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080

char	*server_thonking()
{
	sleep(1);
	return ("response m8\n");
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
	//initing
	int server, cli_sock, reader;
	struct sockaddr_in address = get_addr();
	int opted = 1;
	int address_len = sizeof(address);
	char buffer[1024] = {0};
	char *message = "A message from server !";
	struct pollfd fds;
	int flags;

	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("couldnt aquiere the socket");
		exit(EXIT_FAILURE);
	}
	//De socket non-blocking maken zodat io multiplexing plaats kan vinden
	if ((flags = fcntl(server, F_GETFL)) < 0)
	{
		printf("Couldn't get socket flags !");
		exit(EXIT_FAILURE);
	}
	if ((fcntl(server, F_SETFL, flags | O_NONBLOCK)) < 0)
	{
		printf("Can't make socket non-blocking !");
		exit(EXIT_FAILURE);
	}
	//
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)))
	{
		printf("couldnt get initialize the socket");
		exit(EXIT_FAILURE);
	}
	if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("Binding of socket failed !");
		exit(EXIT_FAILURE);
	}
	//initing END
	if (listen(server, 3) < 0)
	{
		printf("Can't listen from the server !");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		// cli_sock = accept(server, NULL, NULL);
		if ((cli_sock = accept(server, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
		{
			if (errno == EWOULDBLOCK)
			{
				printf("No pending connections; sleeping for one second.\n");
				sleep(1);
			}
			else
			{
				printf("Couldnt accept shit");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			reader = read(cli_sock, buffer, 1024);
			printf("%s\n", buffer);
		}
	}
	// reader = read(cli_sock, buffer, 1024);
	// printf("%s\n", buffer);
	send(cli_sock , message, strlen(message) , 0 );
	printf("Server : Message has been sent ! \n");
	return 0;
}