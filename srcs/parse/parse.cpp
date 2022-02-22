#include <iostream>
#include <fstream>

#include <vector>
#include <deque>

#include <utils.hpp>
#include <Location.hpp>
#include <Server.hpp>

void	setFileInDeque(std::string filename, std::deque<std::string>& filedeque)
{
	std::ifstream			infile;
	std::string				line;

	infile.open(filename);
	while (getline(infile, line))
		filedeque.push_back(line);
}

void	parse(std::string filename, std::vector<Server>& servers)
{
	std::deque<std::string>	filedeque;
	setFileInDeque(filename, filedeque);
	std::vector<std::string>	splitted;

	while (!filedeque.empty())
	{
		splitted = split(filedeque[0]);
		filedeque.pop_front();
		if (splitted.size() == 0)
			continue ;
		if (splitted[0] != "server")
			throw ;
		if (splitted.size() == 1 && filedeque[0] == "{")
		{
			filedeque.pop_front();
			Server server(filedeque);
			servers.push_back(server);
		}
		else if (splitted.size() == 2 && splitted[1] == "{")
		{
			Server server(filedeque);
			servers.push_back(server);
		}
		// throw ;
	}
}