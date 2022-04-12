/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/15 13:47:05 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/12 15:44:53 by avan-ber      ########   odam.nl         */
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
#include <sys/stat.h>

#define BACKLOG 100
#define BUFFER_SIZE 2000

Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request Request)
{
	std::map<std::string, std::string> headers = Request.getHeaders();
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding hostname" << std::endl;
		return NULL;
	}
	std::string host = headers["Host"];
	std::string name = host.substr(0, host.find(":"));
	int port = std::atoi(host.substr(host.find(":") + 1).c_str());
	if (table.find(std::make_pair(port, name)) == table.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding pair" << std::endl;
		return NULL;
	}
	return (table[std::make_pair(port, name)]);
}

std::string	getFileName(const Location& loc)
{
	struct stat	buf;
	
	std::string res = loc.getTitle() + loc.getRoot();
	for (std::vector<std::string>::const_iterator itr = loc.getIndex().begin(); itr != loc.getIndex().end(); itr++)
	{
		std::string filename = res + *itr;
		if (stat(filename.c_str(), &buf))
			return filename;
	}
	/* bad request */
	return NULL;
}

void	remove_last_dir(std::string& request_loc)
{
	if (request_loc.count())
	request_loc = request_loc.substr(0, request_loc.find_last_of("/"));
}

std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>locations, &std::string& request_loc)
{
	while (true)
	{
		if (locations.find(request_loc) != locations.end())
			return locations.find(request_loc);
		remove_last_dir(request_loc);
		if (request_loc.empty())
			return (locations.find("/"));
	}
}

void	handle_response(t_client client, t_data data)
{
	Server *server = find_server(data.table, client.request);
	if (client.request.getType() == GET)
	{
		/* for now */
		std::map<std::string, Location>::const_iterator itr = find_right_location(server->getLocations(), client.request.getLocation());
		if ( itr == server->getLocations().end())
		{
			/* bad request */
			return ;
		}
		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
		{
			/* bad request (405 forbidden)*/
			return ;
		}
		std::string filename = getFileName(itr->second);
		Response response = Response(filename, server);
		int ret = send(client.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
		// std::cout << "----------\n" << response.getResponseBody() << "\n----------" << std::endl;

		// get html locatie
		// formuleer een response header
		// add de html aan de body
		// verstuur naar client
		
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
			case POLLOUT :
				std::cout << "pollout" << std::endl;
				std::cout << client->request.getInput() << std::endl;
				try
				{
					handle_response(*client, data);
					/* code */
				}
				catch(const std::exception& e)
				{
					std::cerr << "\033[31m" << "ERROR: " << e.what() << "\n" << "\033[0m";
				}
				break;
			case LOST_CONNETION :
				std::cout << "lost connection" << std::endl;
				data.clients.erase(data.clients.begin() + (i - data.socket_num));
				data.fds.erase(data.fds.begin() + i);
				break;
		}
		std::cout << "AFTER SOCK: " << i - data.socket_num << std::endl;
	}
}
