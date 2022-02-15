#include "ServerBlock.hpp"
#include <algorithm> /*for_each*/
#include <map>

template <class T>
std::vector<T>	fill(T *arg)
{
	std::vector<T> vec;
	int i = 0;

	for (int i = 0; arg[i]; i++)
		vec.push_back(arg[i]);
	return vec;
}


int main()
{
	std::map< int, std::map<std::string, Server_block> > port_map;
	std::vector<responses>  respons;

	std::string ports[] = {"8070", "8071", "8072"};
	std::string name = "zenotan.nl";
	std::string strs[] = {"~/projects/webserv/files/file1"};

	Server server1 = get_server(&port_map, ports, strs);
	servers.push_back(server1);

	int ports1[] = {8080, 8081, 8082};
	std::string name1 = "api.zenotan.nl";
	std::string strs1[] = {"~/projects/webserv/files/file1"};

	Server server2 = get_server(&port_map, ports1, strs1);
	servers.push_back(server2);

	int ports2[] = {8090, 8091, 8092};
	std::string name2 = "extra.zenotan.nl";
	std::string strs2[] = {"~/projects/webserv/files/file1"};

	Server server3 = get_server(&port_map, ports2, strs2);
	servers.push_back(server3);


	while (true)
	{
		
		
	}

}

// #include <iostream>
// #include <vector>
// #include "includes/utils.hpp"
// #include "Location.hpp"
// #include "Server.hpp"

// int main(int ac, char **av)
// {
// 	try
// 	{
// 		(void)ac;
// 		std::vector<Server>	servers;

// 		parse(av[1], servers);
// 		for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
// 			std::cout << *it <<std::endl;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 	}
// }