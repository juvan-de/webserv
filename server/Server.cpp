#include <Server.hpp>
#include <utils.hpp>
#include <sstream>

void	Server::_errorJumpTable(std::vector<std::string>& line)
{
	if (line[0] == "server")
		throw MissingClosingBracket("Server");
	else
		throw DirectiveNotRecognized(line);

}

void	Server::_checkVarSet()
{
	if (this->_listen.size() == 0)
		this->_listen.insert(80);
	// if (this->_serverName.size() == 0)
	// 	throw MissingServernameInServer();
}

Server::Server (std::deque<std::string>& file, const std::string& curWorkDir)
{
	std::vector<std::string>	splitted;
	
	while (!file.empty())
	{
		splitted = split_on_chars(file[0]);
		file.pop_front();
		if (splitted.size() == 0)
			continue ;
		if (splitted[0] == "}")
		{
			this->_checkVarSet();
			return ;
		}
		else if (splitted[0] == "listen")
			this->setListen(splitted);
		else if (splitted[0] == "server_name")
			this->setServerName(splitted);
		else if (splitted[0] == "error_page")
			this->addErrorPage(splitted);
		else if (splitted[0] != "location")
			this->_errorJumpTable(splitted);
		else if (splitted.size() == 2 && !file.empty())
		{
			std::vector<std::string>	temp = split_on_chars(file[0]);
			if (temp.size() == 1 && temp[0] == "{")
			{
				file.pop_front();
				this->addLocation(file, splitted[1], curWorkDir);
			}
			else
			{
				throw ArgumentIncorrect(temp);
			}
		}
		else if (splitted.size() == 3 && splitted[2] == "{")
		{
			this->addLocation(file, splitted[1], curWorkDir);
		}
		else
		{
			throw ArgumentIncorrect(splitted);
		}
	}
	throw MissingClosingBracket("Server");
}

Server::Server(const Server& ref)
{
	*this = ref;
}

Server&	Server::operator=(const Server& ref)
{
	this->_listen = ref._listen;
	this->_locations = ref._locations;
	this->_errorPage = ref._errorPage;
	this->_serverName = ref._serverName;
	return *this;
}

Server::~Server()
{
	this->_listen.clear();
	this->_locations.clear();
	this->_errorPage.clear();
	this->_serverName.clear();
	return ;
}

/* -Setters- */
void	Server::setListen(std::vector<std::string>& line)
{
	unsigned int	number;

	if (line.size() <= 1)
		throw ArgumentIncorrect(line);
	for (size_t i = 1; i < line.size(); i++)
	{
		if (line[i].find_first_not_of("0123456789") != std::string::npos) // error ipv negeren
			throw ElemNotANumber(line[i], line);
		std::istringstream (line[i]) >> number;
		if (this->_listen.find(number) != this->_listen.end())
			throw DuplicateNumber(line[i], line);
		this->_listen.insert(number);
	}
}

void	Server::addLocation(std::deque<std::string>& file, std::string& title, const std::string& curWorkDir)
{
	Location loc(file, title, curWorkDir);

	this->_locations[title] = loc;
}

void	Server::addErrorPage(std::vector<std::string>& line)
{
	unsigned int number;
	if (line.size() != 3)
		throw ArgumentIncorrect(line);
	if (line[1].find_first_not_of("0123456789") != std::string::npos) 
		throw ElemNotANumber(line[1], line);
	std::istringstream (line[1]) >> number;
	this->_errorPage[number] = line[2];
}

void	Server::setServerName(std::vector<std::string>& line)
{
	if (line.size() <= 1)
		throw ArgumentIncorrect(line);
	this->_serverName = std::set<std::string>(line.begin() + 1, line.end());
}

/* -Getters- */

const std::set<int>&	Server::getListen() const 
{
	return this->_listen;
}

const std::map<std::string, Location>&	Server::getLocations() const
{
	return this->_locations;
}

Location&	Server::getLocation(const std::string& key)
{
	if (this->_locations.find(key) != this->_locations.end())
		return this->_locations[key];
	throw LocationDoesNotExist();
}

const std::map<int, std::string>&	Server::getErrorPages() const
{
	return this->_errorPage;
}

const std::set<std::string>&	Server::getServerName() const
{
	return this->_serverName;
}

std::map<std::string, Location>::const_iterator	Server::getRightLocation(const std::string& request_loc) const
{
	std::map<std::string, Location>::const_iterator	itr = this->_locations.begin();
	std::map<std::string, Location>::const_iterator	best_fitting = this->_locations.end();
	for (; itr != this->_locations.end(); itr++)
	{
		//checken of ie begint met request_loc
		if (request_loc.compare(0, itr->first.length(), itr->first) == 0)
		{
			if (best_fitting == this->_locations.end() || best_fitting->first.size() < itr->first.size())
				best_fitting = itr;
		}
	}
	return best_fitting;
}


std::ostream&	operator<< (std::ostream& out, const Server& obj)
{
	out << "-=- Server -=-" << std::endl;
	out << "listen = [";
	std::set<int> tempListen = obj.getListen();
	for (std::set<int>::iterator it = tempListen.begin(); it != tempListen.end(); it++)
	{
		if (it == tempListen.begin())
			out << *it;
		else
			out << ", " << *it;
	}
	out << "]" << std::endl;
	out << "error_page =";
	std::map<int, std::string> tempErrorPage = obj.getErrorPages();
	for (std::map<int, std::string>::iterator it = tempErrorPage.begin(); it != tempErrorPage.end(); it++)
		out << " {" << it->first << " <-> " << it->second << "}";
	out << std::endl;
	out << "server_name = {";
	std::set<std::string> tempServerName = obj.getServerName();
	for (std::set<std::string>::iterator it = tempServerName.begin(); it != tempServerName.end(); it++)
	{
		if (it == tempServerName.begin())
			out << *it;
		else
			out << ", " << *it;
	}
	out << "}" << std::endl;
	std::map<std::string, Location> tempLocation = obj.getLocations();
	for (std::map<std::string, Location>::iterator it = tempLocation.begin(); it != tempLocation.end(); it++)
		out << it->second << std::endl;
	return out;
}
