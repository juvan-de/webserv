#include <algorithm> /*for_each*/
#include <map>
#include <connection.hpp>
#include <iostream>
#include <vector>
#include <poll.h> // polling
#include <unistd.h> // sleep
#include <fcntl.h> // fcntl

#include <utils.hpp>
#include <webserv.hpp>
#include <connection.hpp> // handle_connection
#include <Location.hpp>
#include <Server.hpp>
#include <data.hpp> // main struct
#include <Socket.hpp> // socket obj

std::set<int> get_ports(std::vector<Server> servers)
{
	std::set<int> ports;

	for (std::vector<Server>::iterator serv_it = servers.begin(); serv_it != servers.end(); serv_it++)
		for (std::set<int>::iterator port_it = serv_it->getListen().begin(); port_it != serv_it->getListen().end(); port_it++)
			ports.insert(*port_it);
	return ports;
}

void	add_cli(std::vector<pollfd> &fds, int cli_sock)
{
	struct pollfd new_fd;
	int flags;

	new_fd.fd = cli_sock;
	new_fd.events = POLLIN;
	flags = fcntl(cli_sock, F_GETFL);
	fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
	fds.push_back(new_fd);
	std::cout << "Connected!" << std::endl;
}

void	check_connection(std::vector<pollfd> &fds, std::vector<Socket> &sockets, int i)
{
	if (fds[i].revents & POLLIN)
	{
		int cli_sock;
		if ((cli_sock = sockets[i].new_connection()) < 0)
		{
			if (errno != EWOULDBLOCK)
				std::cout << "error occured" << std::endl;
		}
		else
			add_cli(fds, cli_sock);
	}
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			(void)ac;
			std::vector<Server>	servers;
			std::set<int> ports;
			std::vector<Socket> sockets;
			std::vector<pollfd> fds;

			parse(av[1], servers);
			ports = get_ports(servers);
		
			std::cout << "initializing ports" << std::endl;
			sockets.reserve(ports.size());
			for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
				sockets.push_back(Socket(*it));

			/* DEBUG */
			// for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
			// 	std::cout << "sock[" << it->getPort() << "], ";
			std::cout << std::endl;

			for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
				fds.push_back(it->getPoll());

			int sockets_num = fds.size();
			/* DEBUG */
			// std::cout << "num " << sockets_num << std::endl;
			while (true)
			{
				for (int i = 0; i < sockets_num; i++)
					fds[i].events = POLLIN;
				// for (std::vector<pollfd>::const_iterator it = fds.begin(); it != fds.end(); it++)
				// 	std::cout << it->fd << std::endl;
				poll(&fds[0], fds.size(), 0);
				handle_connection(fds, servers, sockets_num);
				std::cout << "Waiting for connections..." << std::endl;
				for (int i = 0; i < sockets_num; i++)
					check_connection(fds, sockets, i);
				sleep(1);
				// meerdere keer readen, cgi
			}
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}	
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
