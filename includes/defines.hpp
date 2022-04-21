#ifndef DEFINES_HPP
# define DEFINES_HPP

#include <Server.hpp>
#include <Socket.hpp>
#include <Request.hpp>
#include <poll.h> // pollfd
#include <netinet/in.h> // sockaddr_in
#include <map>

# define LOST_CONNETION 17

class Socket;

typedef struct s_client
{
	int						status;
	int						fd;
	int						opt;
	sockaddr				addr;
	Request					request;

}				t_client;

typedef struct s_data
{
	std::vector<Server>								server_configs;
	std::vector<Socket>								sockets;
	std::map<std::pair<int, std::string>, Server*>	table;
	std::vector<t_client>							clients;
	std::vector<pollfd>								fds;
	int												socket_num;
}				t_data;

std::set<int>	get_ports(std::vector<Server> servers);
void			initialize_data(const std::string filename, t_data &data);

#endif
