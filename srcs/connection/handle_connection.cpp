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
#include <Request.hpp>
#include <Response.hpp>
#include <defines.hpp>
#include <connection.hpp>
#include <sys/stat.h>

#define BACKLOG 100
#define BUFFER_SIZE 2000

void	handle_pollin(t_client &client, pollfd &fd)
{
	// std::cout << "CLIENT FD: [" << client.fd << "]" << std::endl;
	client.request.addto_request(client.fd);
	if (client.request.getType() == NOTSET)
	{
		client.request.setRequest();
		client.request.setHeaders();
	}
	if (client.request.checkIfChunked())
	{
		// std::cout << "CHUNKED" << std::endl;
		client.request.readChunked(client.fd);
	}
	// std::cout << "AFTER PARSIN:\n";
	// std::cout << client.request;
}

void	handle_connection(t_data &data)
{
	int					end = data.fds.size();
	
	for (int i = data.socket_num; i < end; i++)
	{
		try
		{
			if (data.fds[i].revents & POLLOUT)
			{
//				std::cout << "> (DEBUG handle_connection -> pollout) current socket: " << i - data.socket_num << std::endl;
				handle_response(data.clients[i - data.socket_num], data);
			}
			if (data.fds[i].revents & POLLIN)
			{
//				std::cout << "> (DEBUG handle_connection -> pollin) current socket: " << i - data.socket_num << std::endl;
				// std::cout << "client num: " << i << std::endl;
				handle_pollin(data.clients[i - data.socket_num], data.fds[i]);
			}
			if (data.fds[i].revents == LOST_CONNETION)
			{
//				std::cout << "lost connection" << std::endl;
				data.clients.erase(data.clients.begin() + (i - data.socket_num));
				data.fds.erase(data.fds.begin() + i);
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}
