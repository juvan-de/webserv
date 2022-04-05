/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/05 16:46:18 by juvan-de      ########   odam.nl         */
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

#define BACKLOG 100
#define BUFFER_SIZE 2000

// Server	&find_server(std::vector<Server> servers, Request Request)
// {
// 	std::string	host;
// 	std::map<std::string, std::string> Requests = Request.getHeaders();
// 	for (std::map<std::string, std::string>::const_iterator it = Request.getHeaders().begin(); it != Request.getHeaders().end(); it++)
// 	{
// 		if (it->find("Host:", 0, 5) != std::string::npos)
// 		{
// 			host = it->substr(6, std::string::npos);
// 			std::string name = host.substr(0, host.find(":"));
// 			int port = std::atoi(host.substr(host.find(":") + 1).c_str());
// 			for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
// 			{
// 				/* still need to check for correct port as well */
// 				if (it->getServerName().find(name) != it->getServerName().end())
// 				{
// 					return (*it);
// 					Location location = it->getLocation(Request.getLocation());
// 					/* autograbbing the first index entry */
// 					std::string response_file = location.getRoot() + '/' + *(location.getIndex().begin());
// 					// Response response(response_file, *it, Request);
// 					// std::cout << response.getResponse() << std::endl;
// 				}
// 			}
// 		}
// 	}
// 	/* not found statuscode */
// }

// void	handle_response(t_client client, std::vector<Server> servers)
// {
// 		if (client.request.getType() == GET)
// 		{
// 			/* for now */
// 			std::cout << "get request" << std::endl;
// 		//	Server server = find_server(servers, client.request);
// 			Response response = Response(client.request.getLocation(), server);
// 			// std::cout << "----------\n" << response.getResponseBody() << "\n----------" << std::endl;
// 			int ret = send(client.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
// 		}
// 		else if (client.request.getType() == POST)
// 		{
// 			std::cout << "whooo do the post thiing" << std::endl;
// 		}
// 		else if(client.request.getType() == DELETE)
// 		{
// 			std::cout << "we be deletin tho" << std::endl;
// 		}
// 		else
// 		{
// 			std::cout << "shit went wrong yo" << std::endl;
// 		}
// }

void	handle_connection(t_data &data)
{
	t_client			*client;
	int					end = data.fds.size();
	
	for (int i = data.socket_num; i < end; i++)
	{
		client = &data.clients[i - data.socket_num];
		std::cout << "SOCK: " << i - data.socket_num << std::endl;
		switch (data.fds[i].revents)
		{
			case POLLIN :
				std::cout << "pollin" << std::endl;
				client->request.addto_request(client->fd);
				if (client->request.isFinished())
				{
					try 
					{
						client->request.setRequest();
						client->request.setHeaders();
						client->request.checkIfChunked();
						if (client->request.readyForParse())
							data.fds[i].events = POLLOUT;
					}
					catch (const std::exception & e)
					{
						std::cerr << e.what() << std::endl;
					}
				}
				break;
			// case POLLOUT :
			// 	std::cout << "pollout" << std::endl;
			// 	std::cout << client->request.getInput() << std::endl;
			// 	try
			// 	{
			// 		handle_response(*client, data.server_configs);
			// 		/* code */
			// 	}
			// 	catch(const std::exception& e)
			// 	{
			// 		std::cerr << "\033[31m" << "ERROR: " << e.what() << "\n" << "\033[0m";
			// 	}
			// 	break;
			// case LOST_CONNETION :
			// 	std::cout << "lost connection" << std::endl;
			// 	data.clients.erase(data.clients.begin() + (i - data.socket_num));
			// 	data.fds.erase(data.fds.begin() + i);
			// 	break;
		}
		std::cout << "AFTER SOCK: " << i - data.socket_num << std::endl;
	}
}
