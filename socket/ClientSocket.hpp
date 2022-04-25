#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int		_status;
		Request	_request;

	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int domain, int service, int protocol, int port, u_long interface) : Socket(domain, service, protocol, port, interface), _status(200), _request(Request()) {};
};

#endif
