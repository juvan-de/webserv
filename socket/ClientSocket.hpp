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
		int			_isCgi;
		Request		_request;
		sockaddr	_address;


	public:
		/*----------------------------Coplien form----------------------------*/
		ClientSocket(const ClientSocket &ref);
		ClientSocket&	operator=(const ClientSocket &ref);
		~ClientSocket();

		/*--------------------------Member functions--------------------------*/
		ClientSocket(int fd, sockaddr addr);
		sockaddr	&getAddr() { return _address; };
		void		addCgi(int fd);
		void		handle_pollin();
		void		handle_pollout(std::map<std::pair<int, std::string>, Server*> table, Poller &poll);
};

#endif
