#include <ServerSocket.hpp>
#include <BadInit.hpp>
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // close

ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : Socket(domain, service, protocol, port, interface)
{
	/*Instantiating a connection*/
	int flags;

	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &protocol, sizeof(protocol)) < 0) {
		close(getFd());
		throw BadInit();
	}
	if ((flags = fcntl(getFd(), F_GETFL)) < 0)
		throw BadInit();
	if (fcntl(getFd(), F_SETFL, flags | O_NONBLOCK) < 0)
		throw BadInit();

	// Setting up the connection and listening
	if ((bind(getFd(), (struct sockaddr *)&getAddr(), sizeof(getAddr())) < 0))
		throw BadInit();
	if (listen(getFd(), backlog) < 0)
		throw BadInit();
}

ClientSocket	*ServerSocket::check_new_connection()
{
	if (getRevent() & POLLIN)
	{
		ClientSocket *new_cli = new ClientSocket(AF_INET, SOCK_STREAM, 0, getFd(), sizeof(getAddr()));
		return new_cli;
	}
	return NULL;
}
