#include "ServerSocket.hpp"

ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : Socket(domain service, protocol, port, interface)
{
	/*Instantiating a connection*/
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) < 0) {
		close(_fd);
		throw badInit;
	}
	if ((flags = fcntl(_fd, F_GETFL)) < 0)
		throw badInit;
	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw badInit;

	// Setting up the connection and listening
	if ((bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0))
		throw badInit;
	if (listen(_fd, backlog) < 0)
		throw badInit;
}
