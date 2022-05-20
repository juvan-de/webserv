#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <netinet/in.h> // sock_addr
# include <fcntl.h> // fcntl
# include <iostream>
# include <cstddef>

class Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int		_fd;

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket();
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		virtual ~Socket();

		/*--------------------------Member functions--------------------------*/
		Socket(int domain, int service, int protocol);
		Socket(int fd);
		int			new_connection(sockaddr *cli_addr);
		void		setFd(int fd) { _fd = fd; }
		int			getFd() const { return _fd; }

};

#endif
