/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/18 18:01:51 by ztan          ########   odam.nl         */
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

#include <defines.hpp> // data struct, client struct

#define TCP_MAX 1000000
#define BACKLOG 100
#define BUFFER_SIZE 2000

Response	find_response(std::vector<Server> servers, Request Request)
{
	std::string	host;
	std::vector<std::string> Requests = Request.getRequests();
	for (std::vector<std::string>::const_iterator it = Request.getRequests().begin(); it != Request.getRequests().end(); it++)
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
					Location location = it->getLocation(Request.getLocation());
					/* autograbbing the first index entry */
					std::string response_file = location.getRoot() + '/' + *(location.getIndex().begin());
					// Response response(response_file, *it, Request);
					// std::cout << response.getResponse() << std::endl;
					return (Response(response_file, *it));
				}
			}
		}
	}
	return (Response("error_response"));
}

Request		read_request(struct pollfd &fd, std::vector<Server> servers)
{
	char		*request = new char[BUFFER_SIZE + 1];
	std::string	srequest;
	int			ret;

	do 
	{
		ret = read(fd.fd, request, BUFFER_SIZE);
		request[ret] = '\0';
		if (ret)
			srequest.append(request);
	} while (ret > 0);
//	std::cout << srequest << std::endl;
	delete [] request;
	if (srequest.size() > 0)
	{
		Request Request(srequest, fd.fd);
		if (Request.getType() == GET)
		{
			/* for now */
			Response response = find_response(servers, Request);
			Request.setResponse(response);
			std::cout << response.getResponse() << std::endl;
			int ret = send(fd.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
			std::cout << "ret:" << ret << "responselength: " << response.getResponse().length() << std::endl;
			return (Request);
		}
		else if (Request.getType() == POST)
		{
			std::cout << "whooo do the post thiing" << std::endl;
		}
		else if(Request.getType() == DELETE)
		{
			std::cout << "we be deletin tho" << std::endl;
		}
		else
		{
			std::cout << "shit went wrong yo" << std::endl;
		}
	}
	return (Request());
}

void	send_response(std::vector<pollfd> &fds, std::string response, int index)
{
	int bytes_send = 0;
	int len = response.length();

	while (len > 0)
	{
		if (len > TCP_MAX)
		{
			std::cout << "*LEN IS MORE THEN MAX TCP CAN SEND*" << std::endl;
			send(fds[index].fd, response.c_str() + bytes_send, TCP_MAX, 0);
			bytes_send += TCP_MAX;
		}
		else
		{
			send(fds[index].fd, response.c_str() + bytes_send, len, 0);
			bytes_send += len;
		}
		len -= bytes_send;
	}
}

// void	handle_connection(std::vector<Client> clients)
// {	
// 	for (std::vector<Client>::iterator client = clients.begin(); client != clients.end(); client++)
// 	{
// 		if (client.fd & POLLIN)
// 			client.request.addto_request();
// 		else if (client.fd & POLLOUT)
// 			handle_response(client.getRequest(), client.getStatus())
// 	}
// }
