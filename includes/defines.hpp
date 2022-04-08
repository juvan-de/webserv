#ifndef DEFINES_HPP
# define DEFINES_HPP

#include <Server.hpp>
#include <Socket.hpp>
#include <Request.hpp>
#include <poll.h> // pollfd

# define LOST_CONNETION 17

class Socket;

typedef struct s_client
{
	int						fd;
	Request					request;
	int						status;
}				t_client;

typedef struct s_data
{
	std::vector<Server>		server_configs;
	std::vector<Socket>		sockets;
	std::vector<t_client>	clients;
	std::vector<pollfd>		fds;
	int						socket_num;
}				t_data;

std::set<int>	get_ports(std::vector<Server> servers);
void			initialize_data(char *av, t_data &data);

#endif
