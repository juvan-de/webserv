#include "Server.hpp"
#include <algorithm> /* for_each */
#include <unistd.h> /* for sleep idk of het mag */

/*--------------------------------Coplien form--------------------------------*/
Server::Server(std::vector<int> ports, std::vector<std::string> locations) : _locations(locations)
{
	/*Constructor*/
	std::vector<int>::const_iterator	it = ports.begin();
	_timeout = 3 * 60 * 1000; // 3 minuten


	while (it != ports.end())
	{
		Socket	new_port(*it);
		struct pollfd	new_fds;

		_sockets.push_back(new_port);
		new_fds.fd = new_port.getFd();
		new_fds.events = POLLIN;
		_fds.push_back(new_fds);
		it++;
	}
}

Server::~Server()
{
	/*Destructor*/
	// std::for_each(_sockets.begin(), _sockets.end(), &Server::close_sock);
}

Server::Server(const Server &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Server&	Server::operator=(const Server &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

// int		Server::get_sock(int port)
// {
// 	std::cout << "Debug: initializing port " << port << std::endl;

// }

// void	Server::close_sock(int sock_fd)
// {
// 	std::cout << "Debug: closing sock_fd " << sock_fd << std::endl;
// }

// void	Server::accept_connection()
// {
// 	int new_cli;

// //	&data[0] underlying data structure
// 	for (std::vector<Socket>::const_iterator it = _sockets.begin(); it != _sockets.end(); it++)
// 	{
// 		if ((new_cli = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
// 		{
// 			if (errno == EWOULDBLOCK)
// 				sleep(1);
// 			else
// 				throw Server::Accept_err;
// 		}
// 		else
// 		{
// 			std::cout << "Connected!" << std::endl;
// 			_fds.push_back(new_cli);
// 		}
// 	}
			

	
// }

// void	Server::handle_conection()
// {

// }
