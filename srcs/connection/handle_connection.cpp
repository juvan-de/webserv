/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/11 17:26:43 by ztan          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	std::cout << "Socket revent status: pollin" << std::endl;
	client.request.addto_request(client.fd);
	if (!(client.request.checkIfChunked() && !(client.request.readyForParse())))
	{
		client.request.setRequest();
		client.request.setHeaders();
	}
	if (client.request.checkIfChunked())
	{
		std::cout << "CHUNKED" << std::endl;
		client.request.readChunked(client.fd);
	}
	if (client.request.readyForParse())
		fd.events = POLLOUT;
}

void	handle_connection(t_data &data)
{
	int					end = data.fds.size();
	
	for (int i = data.socket_num; i < end; i++)
	{
		try
		{
			switch (data.fds[i].revents)
			{
				case POLLIN :
					std::cout << "> (DEBUG handle_connection -> pollin) current socket: " << i - data.socket_num << std::endl;
					handle_pollin(data.clients[i - data.socket_num], data.fds[i]);
					break;
				case POLLOUT :
					std::cout << "> (DEBUG handle_connection -> pollout) current socket: " << i - data.socket_num << std::endl;
					handle_response(data.clients[i - data.socket_num], data);
					break;
				case LOST_CONNETION :
					std::cout << "lost connection" << std::endl;
					data.clients.erase(data.clients.begin() + (i - data.socket_num));
					data.fds.erase(data.fds.begin() + i);
					break;
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}
