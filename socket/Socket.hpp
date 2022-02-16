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
		int					_port;
		int					_servfd;
		struct sockaddr_in	_address;
		int					_opted;
		int					_address_len;
		int					_flags;
		int					_backlog;

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket();
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		~Socket();

		/*--------------------------Member functions--------------------------*/
		Socket(int port);
		// int		getFd() const;

	private:
		/*--------------------------Exception Classes-------------------------*/
		class badInit : public std::exception
		{
			virtual const char* what() const throw()
			{
				return "Error: could not prepare port";
			}
		} badInit;

		class badAccept : public std::exception
		{
			virtual const char* what() const throw()
			{
				return "Error: could not accept a connection";
			}
		} badAccept;
};

#endif
