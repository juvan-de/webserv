#include <defines.hpp> // data struct, client struct
#include <webserv.hpp> // parse
#include <utils.hpp> // newPoll

std::vector<Socket>	init_sockets(std::set<int> ports)
{
	std::vector<Socket>	sockets;

	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
			sockets.push_back((Socket(*it)));
	return sockets;
}

void	initialize_data(const std::string filename, t_data &data)
{
	std::set<int>	ports;

	try
	{
		std::cout << "Parsing config" << std::endl;
		parse(filename, data.table, ports);
		std::cout << "Initializing ports" << std::endl;
		data.sockets = init_sockets(ports);
		std::cout << "Initializing pollfd array" << std::endl;
		for (std::vector<Socket>::iterator it = data.sockets.begin(); it != data.sockets.end(); it++)
			data.fds.push_back(new_pollfd(it->getFd()));
		data.socket_num = data.fds.size();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}	
}
