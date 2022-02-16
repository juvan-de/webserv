#include <Socket.hpp>
#include <unistd.h> //close
/*--------------------------------Coplien form--------------------------------*/
Socket::Socket()
{
	/*Constructor*/
	return ;
}

Socket::~Socket()
{
	/*Destructor*/
	std::cout << "Debug: closing server_sock " << _servfd << std::endl;
	close(_servfd);
}

Socket::Socket(const Socket &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Socket&	Socket::operator=(const Socket &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int port) : _port(port)
{
	/*Constructor*/
	std::cout << "Initializing port " << _port << std::endl;

	/*Initializing sock address*/
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	_address_len = sizeof(_address);
	_opted = 1;
	_backlog = 100;

	/*Instantiating a connection*/
	if ((_servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw badInit;
	if ((_flags = fcntl(_servfd, F_GETFL)) < 0)
		throw badInit;
	if (fcntl(_servfd, F_SETFL, _flags | O_NONBLOCK) < 0)
		throw badInit;
	if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &_opted, sizeof(_opted)) < 0)
		throw badInit;
	if (bind(_servfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw badInit;
	if (listen(_servfd, _backlog) < 0)
		throw badInit;
}
