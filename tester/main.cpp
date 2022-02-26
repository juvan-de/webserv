#include <sys/socket.h>
#include <arpa/inet.h> // htons
#include <poll.h> // polling
#include <unistd.h> // sleep
#include <fcntl.h> // fcntl

#include <sstream>
#include <cstring>
#include <iostream>
#include <thread>
#include <map>
#include <set>
#include <vector>

#include <utils.hpp>
#include <webserv.hpp>
#include <Location.hpp>
#include <Server.hpp>
#include "cli.hpp"

int	get_num(char *arg)
{
	std::stringstream ss;
	int num = 0;

	std::string str(arg);
	ss << str;
	ss >> num;
	if (ss.fail())
	{
		std::cout << "Argument must be a number" << std::endl;
		return (-1);
	}
	return num;
}

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

int main(int argc, char **argv)
{
	std::map<std::pair<int, std::string>, Server> table;
	std::vector<Server>	servers;
	std::set<int> ports;
	int num = 0;

	if (argc != 3)
		error_check(-1, 1, "invalid args");
	parse(argv[1], servers);
	ports = get_ports(servers, table);
	if ((num = get_num(argv[2])) < 0)
		return 1;
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
		std::cout << "port[" << *it << "] ";
	std::cout << std::endl;

	pthread_t threads[num];
	for (std::set<int>::iterator port = ports.begin(); port != ports.end(); port++)
		for (int i = 0; i < num; i++)
			pthread_create(&threads[i], NULL, client, &const_cast<int&>(*port));
	for (int i = 0; i < num; i++)
		pthread_join(threads[i], NULL);
	return 0;
}