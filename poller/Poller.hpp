#ifndef POLLER_HPP
# define POLLER_HPP

# include <poll.h>
# include <vector>
# include <Socket.hpp>
# include <CgiSocket.hpp>
# include <ClientSocket.hpp>
# include <ServerSocket.hpp>

class CgiSocket;
class ClientSocket;
class ServerSocket;

typedef struct s_poll
{
	ClientSocket	*cli;
	CgiSocket		*cgi;
}	t_poll;

class Poller
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::vector<pollfd>			_cgi_socks; // actualy just a pollstruct, maybe should be put with cli socks
		std::vector<ServerSocket*>	_serv_socks;
		std::vector<ClientSocket*>	_client_socks;

		std::vector<t_poll*>		_sockets;
		std::vector<pollfd>			_pollfds;

	public:
		/*----------------------------Coplien form----------------------------*/
		Poller();
		Poller(const Poller &ref);
		Poller& operator=(const Poller &ref);
		~Poller();

		/*--------------------------Member functions--------------------------*/
		Poller(std::set<int> server_ports);
		pollfd						addPoll(int fd);
		t_poll						*newSocket(CgiSocket *cgi);
		t_poll						*newSocket(ClientSocket *cli);
		ClientSocket				&getSock(int i);
		CgiSocket					&getSock(int i);
		void						deleteSock(int index);

		// pollfd						*preparePoll();
		void						check_server_socks();
		void						execute_poll(std::map<std::pair<int, std::string>, Server*>	table);
		// std::vector<pollfd>			&getCgi() { return _cgi_socks; };
		std::vector<ServerSocket*>	&getServ() { return _serv_socks; };
		std::vector<ClientSocket*>	&getCli() { return _client_socks; };
};

#endif
