#ifndef POLLER_HPP
# define POLLER_HPP

# include <poll.h>
# include <vector>
# include <Socket.hpp>
# include <ClientSocket.hpp>
# include <CgiSocket.hpp>
# include <ServerSocket.hpp>

class ClientSocket;
class CgiSocket;
class ServerSocket;

class Poller
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::vector<CgiSocket*>		_cgi_socks;
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
		Poller(std::set<int> server_ports);
		pollfd						addPoll(int fd);
		void						add_cgi_sock(int fd);

		CgiSocket					&get_cgi_from_index(int i);
		ClientSocket				&get_cli_from_index(int i);

		void						check_server_socks();
		void						check_cli_socks();
		
		void						deleteCgi(int index);
		void						deleteCli(int index);

		pollfd						*preparePoll();
		void						execute_poll(std::map<std::pair<int, std::string>, Server*>	table);

		std::vector<CgiSocket*>		&getCgi() { return _cgi_socks; };
		std::vector<ServerSocket*>	&getServ() { return _serv_socks; };
		std::vector<ClientSocket*>	&getCli() { return _client_socks; };
};

#endif
