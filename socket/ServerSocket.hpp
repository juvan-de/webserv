#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <Socket.hpp>

class ServerSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		// int	_backlog;

	public:
		/*--------------------------Member functions--------------------------*/
		ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog); 
};

#endif
