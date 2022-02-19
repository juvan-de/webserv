#include <algorithm> /*for_each*/
#include <map>
#include <iostream>
#include <vector>
#include <poll.h> // polling
#include <unistd.h> // sleep
#include <fcntl.h> // fcntl

#include <utils.hpp>
#include <webserv.hpp>
#include <Location.hpp>
#include <Server.hpp>
#include <data.hpp> // main struct
#include <Socket.hpp> // socket obj

std::set<int> get_ports(std::vector<Server> servers, std::map<std::pair<int, std::string>, Server>& table)
{
	std::set<int> ports;

	for (std::vector<Server>::iterator serv_it = servers.begin(); serv_it != servers.end(); serv_it++)
	{
		for (std::set<int>::iterator port_it = serv_it->getListen().begin(); port_it != serv_it->getListen().end(); port_it++)
		{
			ports.insert(*port_it);
			for (std::set<std::string>::iterator name_it = serv_it->getServerName().begin(); name_it != serv_it->getServerName().end(); name_it++)
				table.insert(std::make_pair(std::make_pair(*port_it, *name_it), *serv_it));
		}
	}
	return ports;
}

int main(int ac, char **av)
{
	try
	{
		(void)ac;
		std::vector<Server>	servers;
		std::map<std::pair<int, std::string>, Server> table;
		std::set<int> ports;
		std::vector<Socket> sockets;
		std::vector<pollfd> fds;

		parse(av[1], servers);
		ports = get_ports(servers, table);
		std::cout << "return ref of server" << std::endl;
		std::cout << table[std::make_pair(41, "test")] << std::endl;
	
		std::cout << "initializing ports" << std::endl;
		sockets.reserve(ports.size());
		for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
			sockets.push_back(Socket(*it));
		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
			std::cout << "sock[" << it->getPort() << "], ";
		std::cout << std::endl;

		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
			fds.push_back(it->getPoll());
		int sockets_num = fds.size();
		std::cout << "num " << sockets_num << std::endl;
		while (true)
		{
			fds[0].events = POLLIN;
			poll(&fds[0], fds.size(), 3 * 60 * 1000);
			// handle_connection(fds);
			// std::cout << "Waiting for connections..." << std::endl;
			for (int i = 0; i < sockets_num; i++)
			{
				std::cout << "revents: " << fds[i].revents << ", " << POLLNVAL << std::endl;
				if (fds[i].revents & POLLIN)
				{
					std::cout << "debug" << std::endl;
					int cli_sock;
					if ((cli_sock = sockets[i].new_connection()) < 0)
					{
						std::cout << "debug" << std::endl;
						if (errno == EWOULDBLOCK)
						{
							sleep(1);
						}
						else
							std::cout << "error occured" << std::endl;
					}
					else
					{
						// poll <- add accepted client
						struct pollfd new_fd;
						int flags;
						new_fd.fd = cli_sock;
						new_fd.events = POLLIN;
						fds.push_back(new_fd);
						flags = fcntl(cli_sock, F_GETFL);
						fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
						std::cout << "Connected!" << std::endl;
					}
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
