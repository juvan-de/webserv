#include <deque>
#include "Location.hpp"
#include <exception>

// typedef void (Location::*fptr)(std::vector<std::string>&);

// fptr	Location::switchcase(std::string)
// {
// 	switch(tokens[0])
// 	{
// 		case "}":
// 			NULL;
// 		case "root":
// 			return &this->setRoot;
// 		case "client_max_body_size":
// 			return &this->setMaxBodySize;
// 		case "index":
// 			return &this->setIndex;
// 		case "autoindex":
// 			return &this->setAutoIndex;
// 		case "static_dir":
// 			return &this->setStaticDir;
// 		case "cgi":
// 			return &this->setCgi;
// 		case "limit_except":
// 			return &this->setLimitExcept;
// 		case "upload_store":
// 			return &this->setUploadStore;
// 		case "redir":
// 			return &this->setUploadStore;
// 		default:
// 			throw("aanpassen");
// 	}
// }

// Location::Location(std::deque<std::string>& config)
// {
// 	parseLoop(this, config);
// }

// Location::Location(std::vector<std::string> loc)
// {
// 	for (size_t i = 0; i < loc.size(); i++)
// 	{
// 		switch()
// 			case "}":
// 				return ;
// 			case "root":
// 				loc.setRoot();
// 				break ;
// 			case "client_max_body_size":
// 				loc.setMaxBodySize();
// 				break ;
// 			case "index":
// 				loc.setIndex();
// 				break ;
// 			case "autoindex":
// 				loc.setAutoIndex();
// 				break ;
// 			case "static_dir":
// 				loc.setStaticDir();
// 				break ;
// 			case "cgi":
// 				loc.setCgi();
// 				break ;
// 			case "limit_except":
// 				loc.setLimitExcept();
// 				break ;
// 			case "upload_store":
// 				loc.setUploadStore();
// 				break ;
// 			case "redir":
// 				loc.setUploadStore();
// 				break ;
// 			default:
// 				trow();
// 	}
// }

Location&	Location::operator=(const Location& ref)
{
	// this->_title = ref._title;
	this->_root = ref._root;
	// this->_clientMaxBodySize = ref._clientMaxBodySize;
	// this->_index = ref._index;
	// this->_autoindex = ref._autoindex;
	// this->_static_dir = ref._static_dir;
	// this->_cgi = ref._cgi;
	// this->_limitExcept = ref._limitExcept;
	// this->_uploadStore = ref._uploadStore;
	// this->root = ref.root;
	return *this;
}

Location::Location(const Location& ref)
{
	*this = ref;
}

Location::Location()
{
	return ;
}

Location::~Location()
{
	return ;
}

void	Location::setRoot(std::vector<std::string>& line)
{
	// if (line.length() != 2)
	// 	throw ArgumentIncorrect();
	this->_root = line[1];
}

// void	Location::setClientMaxBodySize(std::vector<std::string>& line)
// {
// 	if (line.size() != 2)
// 		throw ArgumentIncorrect(line);
// 	unsigned long number;
// 	unsigned int multiplier = 1;
// 	idx = line[1].find_first_not_of("0123456789")
// 	if (idx != std::string::npos)
// 	{
// 		if (idx != line[1].length() - 1)
// 			throw ; //kies een exception
// 		switch(line[0][idx])
// 		{
// 			case 'k':
// 				multiplier = 1000;
// 			case 'm':
// 				multiplier = 1000000;
// 			case 'g':
// 				multiplier = 1000000000
// 			default:
// 				throw ; // kies een exception
// 		}
// 	}
// 	std::istringstream (line[1]) >> number;
// 	this->_clientMaxBodySize = number * multiplier;
// }

// void	Location::setIndex(std::vector<std::string>& line)
// {
// 	if (line.size() <= 1)
// 		throw ArgumentIncorrect(line);
// 	line.erase(0, 1);
// 	this->_index = line;
// }

// void	Location::setAutoIndex(std::vector<std::string>& line)
// {
// 	if (line.size() != 2)
// 		throw ArgumentIncorrect(line);
// 	switch (line[1])
// 	{
// 		case "on":
// 			this->_autoindex = true;
// 		case "off":
// 			this->_autoindex = false;
// 		default:
// 			throw ;
// 	}
// }

// void	Location::setStaticDir(std::vector<std::string>& line)
// {
// 	if (line.size() != 2)
// 		throw ArgumentIncorrect(line);
// 	switch (line[1])
// 	{
// 		case "true":
// 			this->_staticDir = true;
// 		case "false":
// 			this->_staticDir = false;
// 		default:
// 			throw ;
// 	}
// }

// void	Location::addCgi(std::vector<std::string>& line)
// {
// 	if (line.size() != 3)
// 		throw ArgumentIncorrect(line);
// 	this->_cgi[line[1]] = line[2];
// }

// void	Location::setLimitExcept(std::vector<std::string>& line)
// {
// 	if (line.size() <= 1)
// 		throw ArgumentIncorrect(line);
// 	//nog toevoegen dat ie checked op valid woorden
// 	this->_limitExcept = std::set<std::string>(line.begin() + 1, line.end());
// }

// void	Location::setUploadStore(std::vector<std::string>& line)
// {
// 	if (line.size() != 2)
// 		throw ArgumentIncorrect(line);
// 	this->_uploadStore = line[1];
// }

// void	Location::setRedir(std::vector<std::string>& line)
// {
// 	if (line.size() != 3)
// 		throw ArgumentIncorrect(line);
// 	this->_redir = Redir(line[1], line[2]);
// }

const std::string&	Location::getRoot() const
{
	return this->_root;
}

// unsigned long	Location::getClientMaxBodySize() const
// {
// 	return this->_clientMaxBodySize;
// }

// std::vector<std::string>&	Location::getIndex() const
// {
// 	return this->_Index;
// }

// bool	Location::getAutIndex() const
// {
// 	return this->_autoindex;
// }

// bool	Location::getStaticDir() const
// {
// 	return this->_staticDir;
// }

// std::map<std::string, std::string>&	Location::getCgi() const
// {
// 	return this->_cgi;
// }

// std::set<std::string>&	Location::getLimitExcept() const
// {
// 	return this->_limitExcept;
// }

// std::string&	Location::getUploadStore() const
// {
// 	return this->_uploadStore;
// }

// Redir&	Location::getRedir() const
// {
// 	return this->_redir;
// }

std::ostream&	operator<< (std::ostream& out, const Location& obj)
{
	out << std::boolalpha;
	out << "-=- Location > "; //<< obj.getTitle() << " -=-=-=-" << std::endl;
	out << "root={" << obj.getRoot() << "}" << std::endl;
	// out << "client_max_body_size={" << obj.getClientMaxBodySize() << "}" << std::endl;
	// out << "index={" << obj.getIndex() << "}" << std::endl;
	// out << "autoindex={" << obj.getAutoIndex() << "}" << std::endl;
	// out << "static_dir={" << obj.getStaticDir() << "}" << std::endl;
	// out << "cgi=";
	// std::map<std::string, std::string> temp = obj.getcgi();
	// for (std::map<std::string, std::string>::iterator it = temp.begin(); it != temp.end(); it++)
	// 	out << " {\"" << it.first << "\", \"" << it.last << "}";
	// out << "limit_except=";
	// std::set<std::string> temp = obj.getLimitExcept();
	// for (std::set<std::string>::iterator it = temp.begin(); it != tem.end(); it++)
	// 	out << " {" << *it << "}";
	// out << obj.getRedir() << std::endl;
	return out;
}
