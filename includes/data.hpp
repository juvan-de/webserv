#ifndef DATA_HPP
# define DATA_HPP

#include <Server.hpp>
#include <utility> // pair
#include <poll.h> // pollfd

typedef struct s_data
{
	std::map<std::pair<int, std::string>, Server>	_serverMap;
	std::vector<pollfd>								fds;
}		t_data;

#endif
