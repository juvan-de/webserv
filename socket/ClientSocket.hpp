#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>
# include <Server.hpp>
# include <Poller.hpp>
# include <Response.hpp>
# include <CgiSocket.hpp>

class Poller;
class Server;

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		Request		_request;
		CgiSocket	*_cgi;
		sockaddr_in	_address;
		int			_serverPort;

	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr_in addr, int serverPort);
		int					getPort() const { return _serverPort; }
		const Request		getRequest() const { return _request; }
		CgiSocket			*getCgi() { return _cgi; }
		struct sockaddr_in	&getAddr() { return _address; }
		void				handle_pollin();
		void				handle_pollout(std::map<std::pair<int, std::string>, Server*> table);
		Server				*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request& request);
		Response			handle_get(Server* server, std::map<std::string, Location>::const_iterator location);
		Response			handle_post(Server* server, std::map<std::string, Location>::const_iterator location);
		Response			handle_delete(Server* server, std::map<std::string, Location>::const_iterator location);

};

#endif
