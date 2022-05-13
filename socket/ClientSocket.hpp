#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>
# include <Server.hpp>
# include <Poller.hpp>
# include <Response.hpp>

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
		void			handle_pollin();
		void			handle_pollout(std::map<std::pair<int, std::string>, Server*> table, Poller &poll);
		Response		makeGetResponse(Server* server, std::map<std::string, Location>::const_iterator location);

};

#endif
