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
		// t_cgi		_cgiStatus;
		CgiSocket	*_cgi;


	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr_in addr);
		// t_cgi				getCgiStatus() { return _cgiStatus; }
		CgiSocket			*getCgi() { return _cgi; }
		struct sockaddr_in	&getAddr() { return _address; }
		// void				updateCgiStatus(t_cgi status) { _cgiStatus = status; }
		void				handle_pollin();
		void				handle_pollout(std::map<std::pair<int, std::string>, Server*> table, Poller &poll);
		Response			makeGetResponse(Server* server, std::map<std::string, Location>::const_iterator location);

};

#endif
