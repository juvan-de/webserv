#include <algorithm> /*for_each*/
#include <map>
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

// void	error_check(int err, std::string msg)
// {
// 	if (err < 0)
// 	{
// 		std::cout << "Error: " << msg << std::endl;
// 		exit(EXIT_FAILURE);	
// 	}
// }

// struct sockaddr_in get_addr()
// {
// 	struct sockaddr_in address;

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(8080);
// 	return address;
// }

// int main(int ac, char **av)
// {
// 	try
// 	{
// 		(void)ac;
// 		(void)av;
// 		int server_sock;
// 		struct sockaddr_in address = get_addr();
// 		int opted = 1;
// 		int address_len = sizeof(address);
// 		std::vector<pollfd> fds;
// 		int flags;

// 		/* Getting a socket with ip4 protocol and socket stream */
// 		error_check(server_sock = socket(AF_INET, SOCK_STREAM, 0), "getting the server socket");
// 		/* Getting socket flags with fcntl */
// 		error_check(flags = fcntl(server_sock, F_GETFL), "getting the server socket flags");
// 		/* Setting the non-blocking flag */
// 		error_check(fcntl(server_sock, F_SETFL, flags | O_NONBLOCK), "setting the server socket as non-blocking");
// 		/* Initializing the socket on socket level using the reuseaddr protocol */
// 		error_check(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)), "initializing the server socket");
// 		/* Binding the socket */
// 		error_check(bind(server_sock, (struct sockaddr *)&address, sizeof(address)), "binding the server socket");
// 		/* Start listening and allow a backlog of 100*/
// 		error_check(listen(server_sock, 100), "listening on the server socket");

// 		struct pollfd new_fd;
// 		new_fd.fd = server_sock;
// 		new_fd.events = POLLIN;
// 		fds.push_back(new_fd);
// 		int sockets_num = 1;
// 		while (true)
// 		{
// 			for (int i = 0; i < sockets_num; i++)
// 				fds[i].events = POLLIN;
// 			poll(&fds[0], fds.size(), 3 * 60 * 1000);
// 			std::cout << "debug" << std::endl;
// 			// handle_connection(fds);
// 			std::cout << "Waiting for connections..." << std::endl;
// 			for (int i = 0; i < sockets_num; i++)
// 			{
// 				std::cout << "revents: " << fds[i].revents << ", " << POLLNVAL << std::endl;
// 				if (fds[i].revents & POLLIN)
// 				{
// 					std::cout << "debug" << std::endl;
// 					int cli_sock;
// 					if ((cli_sock = accept(fds[i].fd, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
// 					{
// 						std::cout << "debug" << std::endl;
// 						if (errno == EWOULDBLOCK)
// 						{
// 							sleep(1);
// 						}
// 						else
// 							std::cout << "error occured" << std::endl;
// 					}
// 					else
// 					{
// 						// poll <- add accepted client
// 						struct pollfd new_fd;
// 						int flags;
// 						new_fd.fd = cli_sock;
// 						new_fd.events = POLLIN;
// 						fds.push_back(new_fd);
// 						flags = fcntl(cli_sock, F_GETFL);
// 						fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
// 						std::cout << "Connected!" << std::endl;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
	
// }

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
	
		std::cout << "initializing ports" << std::endl;
		sockets.reserve(ports.size());
		for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
			sockets.push_back(Socket(*it));

		/* DEBUG */
		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
			std::cout << "sock[" << it->getPort() << "], ";
		std::cout << std::endl;

		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
			fds.push_back(it->getPoll());

		int sockets_num = fds.size();
		/* DEBUG */
		std::cout << "num " << sockets_num << std::endl;
		while (true)
		{
			for (int i = 0; i < sockets_num; i++)
				fds[i].events = POLLIN;
			poll(&fds[0], fds.size(), 3 * 60 * 1000);
			handle_connection(fds, sockets_num);
			std::cout << "Waiting for connections..." << std::endl;
			for (int i = 0; i < sockets_num; i++)
			{
				// std::cout << "revents: " << fds[i].events << ", " << POLLIN << ", fd: " << fds[i].fd << std::endl;
				if (fds[i].revents & POLLIN)
				{
					int cli_sock;
					if ((cli_sock = sockets[i].new_connection()) < 0)
					{
						if (errno != EWOULDBLOCK)
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
			sleep(1);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
