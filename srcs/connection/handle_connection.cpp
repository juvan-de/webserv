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
<<<<<<< HEAD
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding pair" << std::endl;
		return NULL;
	}
	return (table[std::make_pair(port, name)]);
}

std::string	getFileName(const Location& loc)
{
	struct stat	buf;
	int			ret;
	std::string res;
	
	if (loc.getTitle().size() == 1 && loc.getTitle()[0])
		res = loc.getTitle() + loc.getRoot();
	else
		res = loc.getTitle() + "/" + loc.getRoot();
	for (std::vector<std::string>::const_iterator itr = loc.getIndex().begin(); itr != loc.getIndex().end(); itr++)
	{
		std::string filename = "." + res + "/" + *itr;
		ret = stat(filename.c_str(), &buf);
		if (ret == 0)
			return filename;
	}
	/* bad request, wa gaan we hier doen */
	std::cout << "bad request (getFileName)" << std::endl;
	return NULL;
}

void	remove_last_dir(std::string& request_loc)
{
	request_loc = request_loc.substr(0, request_loc.find_last_of("/"));
}

std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>& locations, std::string request_loc)
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
			std::cout << "bad request" << std::endl;
			return ;
		}
		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
		{
			/* bad request (405 forbidden)*/
			std::cout << "bad request (405 forbidden)" << std::endl;
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
=======
		// std::cout << "CHUNKED" << std::endl;
		client.request.readChunked(client.fd);
>>>>>>> origin/juvan-de0.2
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
<<<<<<< HEAD
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
				// std::cout << client->request.getInput() << std::endl;
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
=======
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
>>>>>>> origin/juvan-de0.2
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
