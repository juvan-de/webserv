#include <iostream>
#include <fstream>

#include <vector>
#include <deque>

#include <utils.hpp>
#include <Location.hpp>
#include <Server.hpp>

# define COLOR_RED_BOLD				"\033[0;31;01m"
// # define COLOR_RED_BOLD_UNDERLINE	"\033[0;31;01;4m"
// # define COLOR_WHITE_BOLD			"\033[0;37;01m"
// # define COLOR_NORMAL_DIM			"\033[0;02m"
# define COLOR_NORMAL_BOLD			"\033[0;01m"
// # define COLOR_NORMAL				"\033[0m"

static void	setFileInDeque(char* filename, std::deque<std::string>& filedeque)
{
	std::ifstream			infile;
	std::string				line;

	infile.open(filename);
	while (getline(infile, line))
		filedeque.push_back(line);
}

void	parse(char* filename, std::vector<Server>& servers)
{
	std::deque<std::string>	filedeque;
	setFileInDeque(filename, filedeque);
	std::vector<std::string>	splitted;
	size_t filesize = filedeque.size();

	try
	{
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
	catch(const std::exception& e)
	{
		std::cerr << COLOR_NORMAL_BOLD "[line " << filesize - filedeque.size() << "]";
		std::cerr << COLOR_RED_BOLD << " error ";
		std::cerr << e.what() << std::endl;
		return ;
	}
}