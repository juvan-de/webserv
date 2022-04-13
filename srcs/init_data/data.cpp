#include <defines.hpp> // data struct, client struct
#include <webserv.hpp> // parse
#include <utils.hpp> // newPoll

std::set<int> get_ports(std::vector<Server> &servers, std::map<std::pair<int, std::string>, Server*>& table)
{
	std::set<int> ports;

	for (std::vector<Server>::iterator serv_it = servers.begin(); serv_it != servers.end(); serv_it++)
	{
		for (std::set<int>::iterator port_it = serv_it->getListen().begin(); port_it != serv_it->getListen().end(); port_it++)
		{
			ports.insert(*port_it);
			for (std::set<std::string>::iterator name_it = serv_it->getServerName().begin(); name_it != serv_it->getServerName().end(); name_it++)
				table.insert(std::make_pair(std::make_pair(*port_it, *name_it), &(*serv_it)));
		}
	}
	return ports;
}

std::vector<Socket>	init_sockets(std::set<int> ports)
{
	std::vector<Socket>	sockets;

	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
			sockets.push_back((Socket(*it)));
	return sockets;
}

void	initialize_data(char *av, t_data &data)
{
	try
	{
		std::cout << "Parsing config" << std::endl;
		parse(av, data.server_configs);
		
		std::cout << "Initializing ports" << std::endl;
		data.sockets = init_sockets(get_ports(data.server_configs, data.table));
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
