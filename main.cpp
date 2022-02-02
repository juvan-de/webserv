#include "Server.hpp"
#include <algorithm> /*for_each*/

Server	get_server(int *prts, std::string *loc)
{
	std::vector<int>			ports;
	std::vector<std::string>	locs;

	for (int i = 0; prts[i]; i++)
		ports.push_back(prts[i]);
	for (int i = 0; loc[i].empty(); i++)
		locs.push_back(loc[i]);
	Server	serv(ports, locs);
	return serv;
}

int main()
{
	std::vector<Server> servers;
	int ports[] = {8070, 8071, 8072};
	std::string strs[] = {"~/projects/webserv/files/file1"};

	Server server1 = get_server(ports, strs);
	servers.push_back(server1);

	int ports1[] = {8080, 8081, 8082};
	std::string strs1[] = {"~/projects/webserv/files/file1"};

	Server server2 = get_server(ports1, strs1);
	servers.push_back(server2);

	int ports2[] = {8090, 8091, 8092};
	std::string strs2[] = {"~/projects/webserv/files/file1"};

	Server server3 = get_server(ports2, strs2);
	servers.push_back(server3);


	// while (true)
	// {
	// 	std::for_each(servers.begin(), servers.end(), Server::accept_connection);
		
	// }

}