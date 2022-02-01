#include "Server.hpp"
#include <algorithm> /*for_each*/

int main()
{
	std::vector<Server> servers;

	Server::Server server1(std::vector<int>{8080, 8081, 8082},
		 std::vector<std::string>{"~/projects/webserv/files/file1"});
	servers.push_back(server1);

	Server::Server server2(std::vector<int>{8090, 8091, 8092},
		 std::vector<std::string>{"~/projects/webserv/files/file2"});
	servers.push_back(server2);

	Server::Server server3(std::vector<int>{8070, 8071, 8072},
		 std::vector<std::string>{"~/projects/webserv/files/file3"});
	servers.push_back(server3);

	while (true)
	{
		std::for_each(servers.begin(), servers.end(), Server::accept_connection);
		
	}

}