#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>
# include <Server.hpp>
# include <Poller.hpp>
# include <Response.hpp>
# include <CgiSocket.hpp>

class Poller;

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int			_status;
		Request		_request;
		sockaddr_in	_address;
		bool		_hasCgi;
		CgiSocket	*_cgi;


	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr_in addr);
		struct sockaddr_in	&getAddr() { return _address; };
		void				handle_pollin();
		void				handle_pollout(std::map<std::pair<int, std::string>, Server*> table, Poller &poll);
		Response			makeGetResponse(Server* server, std::map<std::string, Location>::const_iterator location);

};

#endif
