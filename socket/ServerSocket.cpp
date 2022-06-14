#include <ServerSocket.hpp>
#include <BadInit.hpp>
#include <BadConnect.hpp>
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // close

ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : Socket(domain, service, protocol)
{
	/*Instantiating a connection*/
	int flags;
	int opt = 1;

	// Set address struct
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = interface;

	// Prepare socket
	if ((flags = fcntl(getFd(), F_GETFL)) < 0)
		throw BadInit();
	if (fcntl(getFd(), F_SETFL, flags | O_NONBLOCK) < 0)
		throw BadInit();
	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw BadInit();

	// Setting up the connection and listening
	if ((bind(getFd(), (struct sockaddr *)&getAddr(), sizeof(sockaddr_in)) < 0))
		throw BadInit();
	if (listen(getFd(), backlog) < 0)
		throw BadInit();
}

ClientSocket	*ServerSocket::get_new_cli()
{
	int			fd;
	sockaddr_in	newaddr;
	socklen_t	addrlen = sizeof(sockaddr);

	// std::cout << "NEW CLI" << std::endl;
	if ((fd = accept(getFd(), (struct sockaddr *)&newaddr, &addrlen)) < 0)
		throw BadConnect();
	return new ClientSocket(fd, newaddr, _port); //memoryleak?
}
