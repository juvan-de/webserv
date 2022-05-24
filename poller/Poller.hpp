#ifndef POLLER_HPP
# define POLLER_HPP

# include <poll.h>
# include <vector>
# include <map>
# include <Socket.hpp>
# include <ClientSocket.hpp>
# include <CgiSocket.hpp>
# include <ServerSocket.hpp>

class ClientSocket;
class CgiSocket;
class ServerSocket;

typedef enum e_type
{
	SERV = 0,
	CLI = 1,
	CGI = 2
}	t_type;

class Poller
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::map<int, ServerSocket*>	_serv_socks;
		std::map<int, ClientSocket*>	_client_socks;
		std::map<int, CgiSocket*>		_cgi_socks;
		std::map<int, t_type>			_lookup;
		std::vector<pollfd>				_pollfds;

		void	changePoll(int oldFd, int newFd);
		void	addSocket(ServerSocket* serv);
		void	addSocket(ClientSocket* cli);
		void	addSocket(CgiSocket* cgi);

		void	handleServ(std::vector< std::pair<int, short> > servers);
		void	handleCli(std::vector< std::pair<int, short> > clients, std::map<std::pair<int, std::string>, Server*> table);
		void	handleCgi(std::vector< std::pair<int, short> > cgi);

		void	deleteSocket(int fd);
	public:
		/*----------------------------Coplien form----------------------------*/
		Poller();
		Poller(const Poller &ref);
		Poller& operator=(const Poller &ref);
		~Poller();

		/*--------------------------Member functions--------------------------*/
		Poller(std::set<int> server_ports);

		void	executePoll(std::map<std::pair<int, std::string>, Server*> table);
};

#endif
