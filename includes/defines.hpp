#ifndef DEFINES_HPP
# define DEFINES_HPP

#include <Server.hpp>
#include <Socket.hpp>
#include <Request.hpp>
#include <poll.h> // pollfd

class Socket;

typedef struct s_data
{
	std::vector<Server>		server_configs;
	std::vector<Socket>		sockets;
	std::vector<struct pollfd>		fds;
	int						socket_num;
}				t_data;

typedef struct s_client
{
	struct pollfd			fd;
	Request					request;
	int						status;
}				t_client;

std::set<int>	get_ports(std::vector<Server> servers);
void			initialize_data(char *av, t_data &data);

#endif
