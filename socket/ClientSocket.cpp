#include <ClientSocket.hpp>
#include <BadInit.hpp>

ClientSocket::ClientSocket(int fd, sockaddr addr) :
	 Socket(fd), _status(200), _request(Request())
{
	int flags;
	int opt = 1;

	// Prepare socket
	if ((flags = fcntl(getFd(), F_GETFL)) < 0)
		throw BadInit();
	if (fcntl(getFd(), F_SETFL, flags | O_NONBLOCK) < 0)
		throw BadInit();
	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw BadInit();

	// Set address struct
	bzero(&_address, sizeof(_address));
	_address = addr;
}

void	ClientSocket::handle_pollin()
{
	std::cout << "POLLING IN" << std::endl;
	_request.addto_request(getFd());
	if (_request.getType() == NOTSET)
	{
		_request.setRequest();
		_request.setHeaders();
	}
	if (_request.checkIfChunked())
	{
		// std::cout << "CHUNKED" << std::endl;
		_request.readChunked(getFd());
	}
}

void	ClientSocket::handle_pollout()
{
	std::cout << "POLLING OUT" << std::endl;
}
