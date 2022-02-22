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
#include <Server.hpp>
#include <Header.hpp>

#define PORT 8080
#define BACKLOG 100
#define BUFFER_SIZE 2000

std::vector<Header>	requests;

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
		Header header(srequest, fd.fd);

		if (header.getType() == GET)
		{
			std::string	root = "files";
			std::string path = root.append(header.getPath());
			header.setResponse(path);
			delete [] request;
			return (header);
		}
	}
	return (Header());
}

void	handle_connection(std::vector<pollfd> &fds, size_t start)
{
	for (size_t i = start; i < fds.size(); i++)
	{
		if (fds[i].revents & POLLIN)
		{
			requests.push_back(read_request(fds[i]));
			fds[i].events = POLLOUT;
		}
		else if (fds[i].revents & POLLOUT)
		{
			for (size_t j = 0; j < requests.size(); j++)
			{
				if (requests[j].getClisock() == fds[i].fd)
				{
					std::string response = requests[j].getResponse();
					send(fds[i].fd, response.c_str(), response.length(), 0);
				}
			}
		}
	}
}
