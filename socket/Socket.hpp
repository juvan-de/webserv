#ifndef SOCKET_HPP
# define SOCKET_HPP

// #include <sys/socket.h> // sockaddr?
#include <fcntl.h> // fcntl
#include <netinet/in.h> // sockaddr_in
#include <set>

#include <defines.hpp> // data and client struct

# include <iostream>
typedef struct s_data t_data;
typedef struct s_client t_client;

class Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int					_fd;
		struct sockaddr_in	_address;

	public:
		/*----------------------------Coplien form----------------------------*/
		Socket();
		Socket(const Socket &ref);
		Socket& operator=(const Socket &ref);
		~Socket();

		/*--------------------------Member functions--------------------------*/
		// AF_INET, SOCK_STREAM, 0, sizeof(adress)
		Socket(int domain, int service, int protocol, int port, u_long interface);
		int					new_connection(sockaddr *cli_addr);
		int					getFd() const { return _fd; }
		struct sockaddr_in	&getAddr() { return _address; };

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

void	check_connection(t_data &data);

#endif
