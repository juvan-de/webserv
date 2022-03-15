/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/15 17:17:45 by juvan-de      ########   odam.nl         */
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
#include <Header.hpp>
#include <Response.hpp>

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

Response	find_response(std::vector<Server> servers, Header header)
{
	std::string	host;
	std::vector<std::string> headers = header.getHeaders();
	for (std::vector<std::string>::const_iterator it = header.getHeaders().begin(); it != header.getHeaders().end(); it++)
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
					Location location = it->getLocation(header.getLocation());
					/* autograbbing the first index entry */
					std::string response_file = location.getRoot() + '/' + *(location.getIndex().begin());
					// Response response(response_file, *it, header);
					// std::cout << response.getResponse() << std::endl;
					return (Response(response_file, *it));
				}
			}
		}
	}
	return (Response("error_response"));
}

Header		read_request(struct pollfd &fd, std::vector<Server> servers)
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
		Header header(srequest, fd.fd);
		if (header.getType() == GET)
		{
			/* for now */
			Response response = find_response(servers, header);
			header.setResponse(response);
			std::cout << response.getResponse() << std::endl;
			int ret = send(fd.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
			std::cout << "ret:" << ret << "responselength: " << response.getResponse().length() << std::endl;
			return (header);
		}
		else if (header.getType() == POST)
		{
			std::cout << "whooo do the post thiing" << std::endl;
		}
		else if(header.getType() == DELETE)
		{
			std::cout << "we be deletin tho" << std::endl;
		}
		else
		{
			std::cout << "shit went wrong yo" << std::endl;
		}
	}
	return (Header());
}

void	handle_connection(std::vector<pollfd> &fds, std::vector<Server> servers, size_t start)
{
	for (size_t i = start; i < fds.size(); i++)
	{
		if (fds[i].revents & POLLIN)
		{
			requests.push_back(read_request(fds[i], servers));
			fds[i].events = POLLOUT;
		}
		else if (fds[i].revents & POLLOUT)
		{
			for (size_t j = start; j < requests.size(); j++)
			{
				if (requests[j].getClisock() == fds[i].fd)
				{
					std::string response = requests[j].getResponse().getResponse();
					std::cout << response << std::endl;
					send(fds[i].fd, response.c_str(), response.length(), 0);
				}
			}
		}
	}
}
