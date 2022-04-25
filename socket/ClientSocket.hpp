#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>
# include <Poller.hpp>

class Poller;

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int			_status;
		Request		_request;
		sockaddr	_address;

	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr addr);
		struct sockaddr	&getAddr() { return _address; };
		void	handle_pollin();
		void	handle_pollout(std::map<std::pair<int, std::string>, Server*> table, Poller &poll);
};

#endif
