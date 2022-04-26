#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>
# include <Request.hpp>
# include <Poller.hpp>
# include <Cgi.hpp>

class Poller;

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int			_status;
		Cgi			_cgi;
		Request		_request;
		sockaddr	_address;
		Server		*_serv;

	public:
		/*----------------------------Coplien form----------------------------*/
		ClientSocket(const ClientSocket &ref);
		ClientSocket&	operator=(const ClientSocket &ref);
		~ClientSocket();

		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr addr);
		Server*		const &getServer() const { return this->_serv; };
		sockaddr	const &getAddr() const { return _address; };

		void		setServer(std::map<std::pair<int, std::string>, Server*> table);
		void		setCgi(int fd);
		void		handle_pollin(std::map<std::pair<int, std::string>, Server*> table);
		void		handle_pollout(Poller &poll);
};

#endif
