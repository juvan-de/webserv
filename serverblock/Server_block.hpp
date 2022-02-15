#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

#include <Socket.hpp>

# include <iostream>
# include <vector>
# include <poll.h>

class Socket;

class Server_block
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::string					_hostname;
		std::vector<std::string>	_locations;

	public:
		/*----------------------------CoPlIeN form----------------------------*/
		Server_block();
		Server_block(const Server_block &ref);
		Server_block& operator=(const Server_block &ref);
		~Server_block();

		/*--------------------------Member functions--------------------------*/
		Server_block(std::string hostname, std::vector<std::string> location);
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
