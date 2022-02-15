#include <algorithm> /*for_each*/
#include <map>
#include <connection.hpp>
#include <iostream>
#include <vector>
#include <utils.hpp>
#include <webserv.hpp>
#include <Location.hpp>
#include <Server.hpp>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <sstream>
#include <fstream>


#define PORT 8080
#define BACKLOG 100
#define BUFFER_SIZE 2000

int main()
{
	std::deque<std::string>	deque;
	char	location[] = "files/configs/config.conf";
	setFileInDeque(location, deque);
	Server	server(deque);
	std::cout << server << std::endl;
	// int server_sock, cli_sock;
	// struct sockaddr_in address = get_addr();
	// int opted = 1;
	// int address_len = sizeof(address);
	// int flags;
	// std::vector<pollfd> fds;
	// struct pollfd new_fd;

	// /* Getting a socket with ip4 protocol and socket stream */
	// error_check(server_sock = socket(AF_INET, SOCK_STREAM, 0), "getting the server socket");
	// /* Getting socket flags with fcntl */
	// error_check(flags = fcntl(server_sock, F_GETFL), "getting the server socket flags");
	// /* Setting the non-blocking flag */
	// error_check(fcntl(server_sock, F_SETFL, flags | O_NONBLOCK), "setting the server socket as non-blocking");
	// /* Initializing the socket on socket level using the reuseaddr protocol */
	// error_check(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)), "initializing the server socket");
	// /* Binding the socket */
	// error_check(bind(server_sock, (struct sockaddr *)&address, sizeof(address)), "binding the server socket");
	// /* Start listening and allow a backlog of 100*/
	// error_check(listen(server_sock, BACKLOG), "listening on the server socket");
	// new_fd.fd = server_sock;
	// new_fd.events = POLLIN;
	// fds.push_back(new_fd);
	// while (true)
	// {
	// 	fds[0].events = POLLIN;
	// 	poll(&fds[0], fds.size(), 3 * 60 * 1000);
	// 	handle_connection(fds);
	// 	//std::cout << "Waiting for connections..." << std::endl;
	// 	if (fds[0].revents & POLLIN)
	// 	{
	// 		(cli_sock = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&address_len));
	// 		if (cli_sock < 0)
	// 		{
	// 			if (errno == EWOULDBLOCK)
	// 				sleep(1);
	// 			else
	// 				error_check(-1, "Accepting a connection");
	// 		}
	// 		else
	// 		{
	// 			// poll <- add accepted client
	// 			struct pollfd new_fd;
	// 			new_fd.fd = cli_sock;
	// 			new_fd.events = POLLIN;
	// 			fds.push_back(new_fd);
	// 			fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
	// 			std::cout << "Connected!" << std::endl;
	// 		}
	// 	}
	// }
	return 0;
}

// int main(int ac, char **av)
// {
// 	try
// 	{
// 		(void)ac;
// 		std::vector<Server>	servers;

// 		parse(av[1], servers);
// 		for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
// 			std::cout << *it <<std::endl;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// }