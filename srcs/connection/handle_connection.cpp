/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/18 18:15:43 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// containers
#include <vector>
// connections
#include <netinet/in.h> // networking
#include <sys/socket.h> // htons
#include <poll.h> // poll
#include <fcntl.h> // setting flags
// idk
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
// cpp idk
#include <iostream> // cout
#include <sstream> // debug
#include <fstream> // files
// custom
#include <Server.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <defines.hpp>

#include <defines.hpp> // data struct, client struct

#define TCP_MAX 1000000
#define BACKLOG 100
#define BUFFER_SIZE 2000

std::vector<Request>	requests;

void	add_cli_to_pollfds(t_data &data, int cli_sock)
{
	struct pollfd new_fd;
	int flags;

	new_fd.fd = cli_sock;
	new_fd.events = POLLIN;
	flags = fcntl(cli_sock, F_GETFL);
	fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
	data.fds.push_back(new_fd);
	std::cout << "Connected!" << std::endl;
}

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
			Server server = find_server(servers, client.request);
			
			std::cout << client.request.getResponse() << std::endl;
			int ret = send(client.fd.fd, client.request.getResponse().getResponse().c_str(), client.request.getResponse().getResponse().length(), 0);
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

void	handle_connection(std::vector<Server> servers, std::vector<t_client> clients)
{
	pollfd clientFD;
	for (std::vector<t_client>::iterator client = clients.begin(); client != clients.end(); client++)
	{
		clientFD = client->fd;
		if (clientFD.revents & POLLIN)
		{
			client->request.addto_request(clientFD.fd);
			if (client->request.isFinished())
				clientFD.events = POLLOUT;
		}
		else if (clientFD.revents & POLLOUT)
		{
			client->request.setRequest();
			handle_response(*client, servers);


			
				// if (requests[j].getClisock() == fds[i].fd)
				// {
				// 	std::string response = requests[j].getResponse().getResponse();
				// 	std::cout << response << std::endl;
				// 	send(fds[i].fd, response.c_str(), response.length(), 0);
				// }
		}
		len -= bytes_send;
	}
}

void	handle_connection(std::vector<Client> clients)
{
	pollfd clientFd;
	
	for (std::vector<Client>::iterator client = clients.begin(); client != clients.end(); client++)
	{
		clientFd = client.getFd();
		
		if (clientFd & POLLIN)
			client.request.addto_request();
		else if (clientFd & POLLOUT)
			handle_response(client.getRequest(), client.getStatus())
	}
}
