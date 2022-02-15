#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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

Server::Server(std::ifstream input)
{

}

Server::Server(const Server& ref)
{

}

Server&	Server::operator=(const Server& ref)
{

}

void	output(struct pollfd fd)
{
	std::cout << "POLLFD OUTPUT: " << fd.fd << ", " << fd.events << std::endl;
}

int main()
{
	int server_sock, cli_sock;
	struct sockaddr_in address = get_addr();
	int opted = 1;
	int address_len = sizeof(address);
	int flags;
	std::vector<pollfd> fds;
	struct pollfd new_fd;

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
	new_fd.fd = server_sock;
	new_fd.events = POLLIN;
	fds.push_back(new_fd);
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
			struct pollfd new_fd;
			new_fd.fd = cli_sock;
			new_fd.events = POLLIN;
			fds.push_back(new_fd);
			std::cout << "Connected!" << std::endl;
			// handle_connection(cli_sock); // <- fd array handelen
		}
		poll(&fds[0], fds.size(), 3 * 60 * 1000);
		std::for_each(fds.begin(), fds.end(), &output);
	}
	return 0;
}
