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
		Server		*_server;
		CgiSocket	*_cgi;
		sockaddr_in	_address;
		int			_serverPort;

	public:
		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr_in addr, int serverPort);
		void				check_cgi();
		int					getPort() const { return _serverPort; }
		const Request		getRequest() const { return _request; }
		CgiSocket			*getCgi() { return _cgi; }
		struct sockaddr_in	&getAddr() { return _address; }
		void				handle_pollin(std::map<std::pair<int, std::string>, Server*>& table);
		void				handle_pollout();
		Server				*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request& request);
		Response 			makeResponse();
		Response			handle_cgi();
		Response			handle_get(std::map<std::string, Location>::const_iterator location);
		Response			handle_post(std::map<std::string, Location>::const_iterator location, const std::string& uri);
		Response			handle_delete(std::map<std::string, Location>::const_iterator location);

};

#endif
