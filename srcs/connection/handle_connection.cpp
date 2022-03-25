/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/25 19:19:44 by ztan          ########   odam.nl         */
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

#define PORT 8080
#define BACKLOG 100
#define BUFFER_SIZE 2000

Server	&find_server(std::vector<Server> servers, Request Request)
{
	std::string	host;
	std::vector<std::string> Requests = Request.getHeaders();
	for (std::vector<std::string>::const_iterator it = Request.getHeaders().begin(); it != Request.getHeaders().end(); it++)
	{
		if (it->find("Host:", 0, 5) != std::string::npos)
		{
			host = it->substr(6, std::string::npos);
			std::string name = host.substr(0, host.find(":"));
			int port = std::atoi(host.substr(host.find(":") + 1).c_str());
			for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
			{
				/* still need to check for correct port as well */
				if (it->getServerName().find(name) != it->getServerName().end())
				{
					return (*it);
					Location location = it->getLocation(Request.getLocation());
					/* autograbbing the first index entry */
					std::string response_file = location.getRoot() + '/' + *(location.getIndex().begin());
					// Response response(response_file, *it, Request);
					// std::cout << response.getResponse() << std::endl;
				}
			}
		}
	}
	/* not found statuscode */
}

void	handle_response(t_client client, std::vector<Server> servers)
{
		if (client.request.getType() == GET)
		{
			/* for now */
			std::cout << "get request" << std::endl;
			// std::cout << "*REQUEST*\n" << client.request.getLocation() << "\n*REQUEST*" << std::endl;
			Server server = find_server(servers, client.request);
			Response response = Response(client.request.getLocation(), server);
			std::cout << "----------\n" << response.getResponseBody() << "\n----------" << std::endl;
			int ret = send(client.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
		}
		else if (client.request.getType() == POST)
		{
			std::cout << "whooo do the post thiing" << std::endl;
		}
		else if(client.request.getType() == DELETE)
		{
			std::cout << "we be deletin tho" << std::endl;
		}
		else
		{
			std::cout << "shit went wrong yo" << std::endl;
		}	
}

void	handle_connection(t_data &data)
{
	std::vector<pollfd> fds = data.fds;
	int					end = data.fds.size();

	for (int i = data.socket_num; i < end; i++)
	{
		switch (data.fds[i].revents)
		{
			case POLLIN :
				std::cout << "pollin" << std::endl;
				data.clients[i - data.socket_num].request.addto_request(data.clients[i - data.socket_num].fd);
				if (data.clients[i - data.socket_num].request.isFinished())
					data.fds[i].events = POLLOUT;
				break;
			case POLLOUT :
				std::cout << "pollout" << std::endl;
				data.clients[i - data.socket_num].request.setRequest();
				handle_response(data.clients[i - data.socket_num], data.server_configs);
				break;
			case LOST_CONNETION :
				std::cout << "lost connection" << std::endl;
				data.clients.erase(data.clients.begin() + (i - data.socket_num));
				data.fds.erase(data.fds.begin() + i);
				break;
		}
	}
}
