#ifndef SOCKET_HPP
# define SOCKET_HPP

// #include <sys/socket.h> // sockaddr?
#include <fcntl.h> // fcntl
#include <poll.h> // polling
#include <netinet/in.h> // sockaddr_in
#include <set>

#include <defines.hpp> // data and client struct

# include <iostream>
# define BACKLOG 100

typedef struct s_data t_data;
typedef struct s_client t_client;

class Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int					_port;
		int					_servfd;
		int					_opted;
		int					_address_len;
		int					_flags;
		int					_backlog;
		struct sockaddr_in	_address;
		struct pollfd		_poll;

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket();
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		~Socket();

		/*--------------------------Member functions--------------------------*/
		Socket(int port);
		int					new_connection();
		int					getPort() const { return _port; }
		int					getFd() const { return _servfd; }
		struct sockaddr_in	&getAddr() { return _address; };
		int					&getAddrlen() { return _address_len; }
		struct pollfd		&getPoll() {return _poll; }
		int					getFlags() const { return _flags; }

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

void	check_connection(t_data &data, std::vector<t_client*> &clients, int i);

#endif
