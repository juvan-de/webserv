#include "Socket.hpp"
#include <fstream> //close
/*--------------------------------Coplien form--------------------------------*/
Socket::Socket(int port)
{
	/*Constructor*/
	int flags;
	int opted = 1;

	_sock_addr.sin_family = AF_INET;
	_sock_addr.sin_addr.s_addr = INADDR_ANY;
	_sock_addr.sin_port = htons(port);

	/* Getting a socket with ip4 protocol and socket stream */
	error_check(_server_sock = socket(AF_INET, SOCK_STREAM, 0),
			"getting the server socket");
	/* Getting socket flags with fcntl */
	error_check(flags = fcntl(_server_sock, F_GETFL),
			"getting the server socket flags");
	/* Setting the non-blocking flag */
	error_check(fcntl(_server_sock, F_SETFL, flags | O_NONBLOCK),
			"setting the server socket as non-blocking");
	/* Initializing the socket on socket level using the reuseaddr protocol */
	error_check(setsockopt(_server_sock, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)),
			"initializing the server socket");
	/* Binding the socket */
	error_check(bind(_server_sock, (struct sockaddr *)&_sock_addr, sizeof(_sock_addr)),
			"binding the server socket");
	/* Start listening and allow a backlog of 100*/
	error_check(listen(_server_sock, BACKLOG),
			"listening on the server socket");
}

Socket::~Socket()
{
	/*Destructor*/
	close(_server_sock);
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

std::ostream&	operator<<(std::ostream &out, const Socket &ref)
{
	/*Output operator*/
	/*out << Socket;
	return out;*/
}