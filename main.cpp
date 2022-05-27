#include <iostream> // cout
#include <unistd.h> // sleep
#include <csignal> // signal for ignoring pipe breaks
#include <webserv.hpp> // parse
#include <Poller.hpp>



std::set<int> getPortsAndSetTable(std::vector<Server>& servers, std::map<std::pair<int, std::string>, Server*>& table)
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

int main(int ac, char **av)
{
	if (ac == 2)
	{
		std::map<std::pair<int, std::string>, Server*>	table;
		std::vector<Server>								server_configs;
		std::set<int>									ports;
		Poller											*poller;

		signal(SIGINT, NULL);
		try
		{
			parse(av[1], server_configs);
			ports = getPortsAndSetTable(server_configs, table);
			poller = new Poller(ports);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return (0);	 // "als poll niet goed construct dan closen voordat aborts of segv gebeuren"
		}
		while (true)
		{
			poller->executePoll(table);
			// std::cout << "Waiting for connections..." << std::endl;
			usleep(500);
		}
		if (poller)
			delete(poller);
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
