#include <vector>
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

#include "Header.hpp"

#define PORT 8080
#define BACKLOG 100
#define BUFFER_SIZE 2000

std::vector<Header>	requests;

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

Header		read_request(struct pollfd &fd)
{
	char	*request = new char[BUFFER_SIZE + 1];
	int ret = read(fd.fd, request, BUFFER_SIZE);
	request[ret] = '\0';
	if (ret > 0)
	{
		std::string srequest(request);
		Header header(srequest);

		if (header.getType() == GET)
		{
			std::string	root = "files";
			std::string path = root.append(header.getPath());
			char *cstring = new char[path.length() + 1];
			std::strcpy(cstring, path.c_str());
			header.setResponse(path);
			/* how do I get poll or smth to set the clientfd to pollout?(to receive back the response) :( */
			delete [] request;
			return (header);
		}
	}
	return (Header());
}

void	handle_connection(std::vector<pollfd> &fds)
{
	for (size_t i = 1; i < fds.size(); i++)
	{
		if (fds[i].revents & POLLIN)
		{
			requests.push_back(read_request(fds[i]));
			//char *response = requests[0].getResponse();
			//send(fds[i].fd, requests[0].getResponse(), std::strlen(requests[0].getResponse()), 0);
		}
		else if (fds[i].revents & POLLOUT)
		{
			std::string response = requests[0].getResponse();
			char *cstring = new char[response.length() + 1];
			std::strcpy(cstring, response.c_str());
			send(fds[i].fd, cstring, std::strlen(cstring), 0);
			//std::cout << "I need to write something to the thingy" << std::endl;
		}
	}
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
			// poll <- add accepted client
			struct pollfd new_fd;
			new_fd.fd = cli_sock;
			new_fd.events = POLLIN;
			fds.push_back(new_fd);
			std::cout << "Connected!" << std::endl;
		}
		poll(&fds[0], fds.size(), 3 * 60 * 1000);
		handle_connection(fds);
	}
	return 0;
}
