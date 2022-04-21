#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <Socket.hpp>
# include <ClientSocket.hpp>

class ClientSocket;

class ServerSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/

	public:
		/*--------------------------Member functions--------------------------*/
		ServerSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
		ClientSocket	check_new_connection(sockaddr *cli_addr);
};

#endif
