#include <algorithm> /*for_each*/
#include <map>

#include <iostream>
#include <vector>
#include <utils.hpp>
#include <webserv.hpp>
#include <Location.hpp>
#include <Server.hpp>
#include <data.hpp> // main struct

std::set<int> get_ports(std::vector<Server> servers, std::map<std::pair<int, std::string>, Server>& table)
{
	std::set<int> ports;

	for (std::vector<Server>::iterator serv_it = servers.begin(); serv_it != servers.end(); serv_it++)
	{
		for (std::set<int>::iterator port_it = serv_it->getListen().begin(); port_it != serv_it->getListen().end(); port_it++)
		{
			ports.insert(*port_it);
			for (std::set<std::string>::iterator name_it = serv_it->getServerName().begin(); name_it != serv_it->getServerName().end(); name_it++)
				table.insert(std::make_pair(std::make_pair(*port_it, *name_it), *serv_it));
		}
	}
	return ports;
}

int main(int ac, char **av)
{
	try
	{
		(void)ac;
		std::vector<Server>	servers;
		std::map<std::pair<int, std::string>, Server> table;
		std::set<int> ports;

		parse(av[1], servers);
		ports = get_ports(servers, table);
		std::cout << "return ref of server" << std::endl;
		std::cout << table[std::make_pair(41, "test")] << std::endl;
		std::cout << "print ports" << std::endl;
		for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
			std::cout << *it << ", ";
		std::cout << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
