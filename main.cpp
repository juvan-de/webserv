#include <iostream> // cout
#include <unistd.h> // sleep
#include <csignal> // signal for ignoring pipe breaks
// custom
// #include <connection.hpp> // handle_connection
#include <webserv.hpp> // parse
#include <Poller.hpp>

std::vector<int> get_ports(std::vector<Server> &servers, std::map<std::pair<int, std::string>, Server*>& table)
{
	std::vector<int> ports;

	for (std::vector<Server>::iterator serv_it = servers.begin(); serv_it != servers.end(); serv_it++)
	{
		for (std::set<int>::iterator port_it = serv_it->getListen().begin(); port_it != serv_it->getListen().end(); port_it++)
		{
			ports.push_back(*port_it);
			for (std::set<std::string>::iterator name_it = serv_it->getServerName().begin(); name_it != serv_it->getServerName().end(); name_it++)
				table.insert(std::make_pair(std::make_pair(*port_it, *name_it), &(*serv_it)));
		}
	}
	return ports;
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		std::map<std::pair<int, std::string>, Server*>	table;
		std::vector<Server>		server_configs;
		std::vector<int>		ports;
		Poller					poller;

		try
		{
			std::cout << "Parsing config" << std::endl;
			parse(av[1], server_configs);
			ports = get_ports(server_configs, table);
			poller = Poller(ports);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		while (true)
		{
			poller.execute_poll(table);
			// handle_connection(data);
			std::cout << "Waiting for connections..." << std::endl;
			// check_connection(data);
			sleep(1);
		}
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
