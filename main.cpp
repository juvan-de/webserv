#include "ServerBlock.hpp"
#include <algorithm> /*for_each*/
#include <map>

#include <iostream>
#include <vector>
#include "includes/utils.hpp"
#include "Location.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
	try
	{
		(void)ac;
		std::vector<Server>	servers;

		parse(av[1], servers);
		for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
			std::cout << *it <<std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
