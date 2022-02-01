#include "Server.hpp"
#include <algorithm> /*for_each*/

/*--------------------------------Coplien form--------------------------------*/
Server::Server(std::vector<int> ports, std::vector<std::string> locations) : _locations(locations)
{
	/*Constructor*/
	std::vector<int>::const_iterator it = ports.begin();

	while (it != ports.end())
	{
		_sockets.push_back(get_sock(*it));
		it++;
	}
}

Server::~Server()
{
	/*Destructor*/
	std::for_each(_sockets.begin(), _sockets.end(), &close_sock);
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

int		Server::get_sock(int port)
{
	std::cout << "Debug: initializing port " << port << std::endl;

}

void	Server::close_sock(int sock_fd)
{
	std::cout << "Debug: closing sock_fd " << sock_fd << std::endl;
}

void	Server::accept_connection()
{
	int new_cli;

	if ((new_cli = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&address_len)) < 0)
	{
		if (errno == EWOULDBLOCK)
			sleep(1);
		else
			throw Server::Accept_err;
	}
	else
	{
		std::cout << "Connected!" << std::endl;
		_fds.push_back(new_cli);
	}
}

void	Server::handle_conection()
{

}
