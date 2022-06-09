#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <Socket.hpp>
# include <ClientSocket.hpp>

#define BACKLOG 100

class ClientSocket;

class ServerSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		sockaddr_in	_address;

	public:
		/*--------------------------Member functions--------------------------*/
		ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
		ClientSocket		*get_new_cli();
		struct sockaddr_in	&getAddr() { return _address; };
};

#endif
