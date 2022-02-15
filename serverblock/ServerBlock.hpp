#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include <Socket.hpp>

# include <iostream>
# include <vector>
# include <poll.h>

typedef class s_ServerBlock
{
		std::string					_hostname;
		std::vector<std::string>	_locations;
}	t_ServerBlock;

#endif
