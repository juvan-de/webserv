#include <iostream>
#include <fstream>

#include <vector>
#include <deque>

#include <utils.hpp>
#include <Location.hpp>
#include <Server.hpp>
#include <webservException.hpp>

# define COLOR_RED_BOLD				"\033[0;31;01m"
// # define COLOR_RED_BOLD_UNDERLINE	"\033[0;31;01;4m"
// # define COLOR_WHITE_BOLD			"\033[0;37;01m"
// # define COLOR_NORMAL_DIM			"\033[0;02m"
# define COLOR_NORMAL_BOLD			"\033[0;01m"
// # define COLOR_NORMAL				"\033[0m"

bool	checkFileName(const std::string filename)
{
	if (filename.size() <= 5)
		return false;
	if (filename.substr(filename.size() - 5) != ".conf")
		return false;
	return true;
}

static void	setFileInDeque(const std::string filename, std::deque<std::string>& filedeque)
{
	std::ifstream			infile;
	std::string				line;
	std::size_t				found;

	if (!checkFileName(filename))
		throw WrongFile(filename);
	infile.open(filename);
	if (infile.std::ios::fail())
		throw WrongFile(filename);
	while (getline(infile, line))
	{
		if ((found = line.find("#")) != std::string::npos)
			line.resize(found);
		if((found = line.find_first_not_of(" \t\n\v\f\r")) != std::string::npos)
			filedeque.push_back(line);
	}
}

void	parse(const std::string filename, std::vector<Server>& servers)
{
	std::deque<std::string>	filedeque;

	try
	{
		setFileInDeque(filename, filedeque);
	}
	catch(const std::exception& e)
	{
		std::cerr << COLOR_RED_BOLD << "error ";
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	size_t filesize = filedeque.size();
	try
	{
		std::vector<std::string>	splitted;
		while (!filedeque.empty())
		{
			splitted = split(filedeque[0]);
			filedeque.pop_front();
			if (splitted.size() == 0)
				continue ;
			if (splitted[0] != "server")
				throw ;
			if ((splitted.size() == 1 && filedeque[0] == "{") || (splitted.size() == 2 && splitted[1] == "{"))
			{
				if (splitted.size() == 1 && filedeque[0] == "{")
					filedeque.pop_front();
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
		exit(1);
	}
}