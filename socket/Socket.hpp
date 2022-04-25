#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <netinet/in.h> // sock_addr
# include <fcntl.h> // fcntl
# include <poll.h> // pollfd
# include <iostream>
# include <cstddef>

class Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int			_fd;
		sockaddr_in	_address;
		pollfd		_poll;
		Socket() {};

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		virtual ~Socket();

		/*--------------------------Member functions--------------------------*/
		// AF_INET, SOCK_STREAM, 0, sizeof(adress)
		Socket(int domain, int service, int protocol, int port, u_long interface);
		int					new_connection(sockaddr *cli_addr);
		int					getFd() const { return _fd; }
		short				getRevent() const { return _poll.revents; };
		pollfd				&getPoll() { return _poll; };
		struct sockaddr_in	&getAddr() { return _address; };
};

#endif
