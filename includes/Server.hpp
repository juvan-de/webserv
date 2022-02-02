#ifndef SERVER_HPP
# define SERVER_HPP

#include <Socket.hpp>

# include <iostream>
# include <vector>
# include <poll.h>

class Socket;

class Server
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::vector<Socket>			_sockets;
		std::vector<pollfd>			_fds;
		int							_timeout;
		std::vector<std::string>	_locations;

	public:
		/*----------------------------CoPlIeN form----------------------------*/
		Server();
		Server(const Server &ref);
		Server& operator=(const Server &ref);
		~Server();

		/*--------------------------Member functions--------------------------*/
		Server(std::vector<int> sockets, std::vector<std::string> location);
		// int							get_sock(int port);
		// void						close_sock(int sock_fd);
		// void						accept_connection();
		// void						handle_conection();

		/*--------------------------Exception Classes-------------------------*/
		class Accept_err : public std::exception
		{
			virtual const char* what() const throw()
			{
				return "Error: something went wrong accepting a connection";
			}
		} Accept_err;
};

#endif
