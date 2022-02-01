#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>

# include <iostream>
# define BACKLOG 100

class Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int					_server_sock;
		struct sockaddr_in	_sock_addr;

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket(int port);
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		~Socket();

		/*--------------------------Member functions--------------------------*/

};

std::ostream&	operator<<(std::ostream &out, const Socket &ref);

#endif
