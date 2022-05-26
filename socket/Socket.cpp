#include <Socket.hpp>
#include <BadInit.hpp>
#include <vector>
#include <errno.h> // erno, not needed
#include <unistd.h> // close
#include <errno.h>

/*--------------------------------Coplien form--------------------------------*/
Socket::~Socket()
{
	/*Destructor*/
	std::cout << "Debug: closing sock " << _fd << std::endl;
	close(_fd);
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int domain, int service, int protocol)
{
    // Establish a connection
	if ((_fd = socket(domain, service, protocol)) < 0)
		throw BadInit();
	std::cout << "Debug: constructing sock " << _fd << std::endl;
}

Socket::Socket(int fd) : _fd(fd)
{
	std::cout << "Debug: constructing sock " << _fd << std::endl;
}