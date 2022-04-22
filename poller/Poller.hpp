#ifndef POLLER_HPP
# define POLLER_HPP

# include <iostream>
# include <vector>
# include <Socket.hpp>
# include <ClientSocket.hpp>
# include <ServerSocket.hpp>

class Poller
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::vector<pollfd>			_cgi_socks; // actualy just a pollstruct
		std::vector<ServerSocket*>	_serv_socks;
		std::vector<ClientSocket*>	_client_socks; 
		std::vector<pollfd>			_pollfds;

	public:
		/*----------------------------Coplien form----------------------------*/
		Poller();
		Poller(const Poller &ref);
		Poller& operator=(const Poller &ref);
		~Poller();

		/*--------------------------Member functions--------------------------*/
		Poller(std::vector<int>	server_ports);
		void						add_cgi_sock(int fd);
		void						execute_poll();
		std::vector<pollfd>			&getCgi() { return _cgi_socks; };
		std::vector<ServerSocket*>	&getServ() { return _serv_socks; };
		std::vector<ClientSocket*>	&getCli() { return _client_socks; };
};

#endif
