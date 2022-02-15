#include <Server.hpp>
#include <utils.hpp>
#include <sstream>

Server::Server()
{
	return ;
}

Server::Server(std::deque<std::string>& file)
{
	std::vector<std::string>	splitted;
	
	while (!file.empty())
	{
		splitted = split(file[0]);
		file.pop_front();
		if (splitted.size() == 0)
			continue ;
		if (splitted[0] == "}")
			return ;
		else if (splitted[0] == "listen")
			this->setListen(splitted);
		else if (splitted[0] == "server_name")
			this->setServerName(splitted);
		else if (splitted[0] == "error_page")
			this->addErrorPage(splitted);
		else if (splitted[0] == "location")
		{
			if (splitted.size() == 3 && splitted[2] == "{")
			{
				this->addLocation(file, splitted[1]);
				continue ;
			}
			else if (splitted.size() == 2)
			{
				if (!file.empty() && file[0] == "{")
				{
					file.pop_front();
					this->addLocation(file, splitted[1]);
					continue ;
				}
			}
			throw ArgumentIncorrect();
		}
		throw ElemNotRecognized() ;
	} 
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
	return ;
}

/* -Setters- */

void	Server::setListen(std::vector<std::string>& line)
{
	unsigned int	number;
	if (line.size() <= 1)
		throw ArgumentIncorrect();
	for (size_t i = 1; i < line.size(); i++)
	{
		if (line[i].find_first_not_of("0123456789") != std::string::npos)
			continue ;
		std::istringstream (line[i]) >> number;
		this->_listen.insert(number);
	}
}

void	Server::addLocation(std::deque<std::string>& file, std::string& title)
{
	Location loc(file, title);

	this->_locations[title] = loc;
}

void	Server::addErrorPage(std::vector<std::string>& line)
{
	unsigned int number;
	if (line.size() != 3)
		throw ArgumentIncorrect();
	if (line[1].find_first_not_of("0123456789") == std::string::npos)
		throw NotANumber();
	std::istringstream (line[1]) >> number;
	this->_errorPage[number] = line[2];
}

void	Server::setServerName(std::vector<std::string>& line)
{
	if (line.size() <= 1)
		throw ArgumentIncorrect();
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

// Location&	Server::getLocation(const std::string& key)
// {
// 	if (this->_locations.find(key) != this->_locations.end())
// 		return this->_locations[key];
// }

const std::map<int, std::string>&	Server::getErrorPage() const
{
	return this->_errorPage;
}

const std::set<std::string>&	Server::getServerName() const
{
	return this->_serverName;
}

std::ostream&	operator<< (std::ostream& out, const Server& obj)
{
	out << "-=- Server -=-" << std::endl;
	out << "listen= [";
	std::set<int> tempListen = obj.getListen();
	for (std::set<int>::iterator it = tempListen.begin(); it != tempListen.end(); it++)
	{
		if (it == tempListen.begin())
			out << *it;
		else
			out << ", " << *it;
	}
	out << "]" << std::endl;
	out << "error_page=";
	std::map<int, std::string> tempErrorPage = obj.getErrorPage();
	for (std::map<int, std::string>::iterator it = tempErrorPage.begin(); it != tempErrorPage.end(); it++)
		out << " {" << it->first << " <-> " << it->second << "}";
	out << std::endl;
	out << "server_name= {";
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
