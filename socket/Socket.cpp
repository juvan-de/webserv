#include <Socket.hpp>
#include <BadInit.hpp>
#include <vector>
#include <errno.h> // erno, not needed
#include <unistd.h> // close
/*--------------------------------Coplien form--------------------------------*/
Socket::Socket()
{
	/*Constructor*/
	return ;
}

Socket::~Socket()
{
	/*Destructor*/
	std::cout << "Debug: closing server_sock " << _fd << std::endl;
	close(_fd);
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
		_fd = ref._fd;
		_address = ref._address;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	// Set address struct
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = interface;

    // Establish a connection
	if ((_fd = socket(domain, service, protocol)) < 0)
		throw BadInit();

	// Set poll
	bzero(&_poll, sizeof(_poll));
	_poll.fd = _fd;
	_poll.events = POLLIN | POLLOUT;
}
