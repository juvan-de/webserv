#include <Socket.hpp>
#include <BadInit.hpp>
#include <vector>
#include <errno.h> // erno, not needed
#include <unistd.h> // close
/*--------------------------------Coplien form--------------------------------*/
Socket::Socket()
{
	/*Constructor*/
	std::cout << "Debug: constructing sock " << _fd << std::endl;
	return ;
}

Socket::~Socket()
{
	/*Destructor*/
	std::cout << "Debug: closing sock " << _fd << std::endl;
	close(_fd);
}

Socket::Socket(const Socket &ref)
{
	/*Copy constructor*/
	std::cout << "Debug: copy sock " << _fd << std::endl;
	*this = ref;
}

Socket&	Socket::operator=(const Socket &ref)
{
	/*Assignation operator*/
	std::cout << "Debug: =opp sock " << _fd << std::endl;
	if (this != &ref)
	{
		_fd = ref._fd;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int domain, int service, int protocol)
{
    // Establish a connection
	if ((_fd = socket(domain, service, protocol)) < 0)
		throw BadInit();
	std::cout << "Debug: constructing sock " << _fd << std::endl;
}

Socket::Socket(int fd) : _fd(fd) {}