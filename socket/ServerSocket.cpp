#include <ServerSocket.hpp>
#include <BadInit.hpp>
#include <BadConnect.hpp>
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // close

/*--------------------------------Coplien form--------------------------------*/
ServerSocket::ServerSocket(const ServerSocket &ref) : Socket(ref)
{
	/*Copy constructor*/
	*this = ref;
}

ServerSocket&	ServerSocket::operator=(const ServerSocket &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		_address = ref._address;
	}
	return *this;
}

ServerSocket::~ServerSocket()
{
	/*Destructor*/
}
/*--------------------------------Coplien form--------------------------------*/

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
	sockaddr	newaddr;
	socklen_t	addrlen = sizeof(sockaddr);

	if ((fd = accept(getFd(), (struct sockaddr *)&newaddr, &addrlen)) < 0)
		throw BadConnect();
	return new ClientSocket(fd, newaddr);
}
