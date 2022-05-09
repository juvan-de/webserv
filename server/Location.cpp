#include <deque>
#include <utils.hpp>
#include <Location.hpp>
#include <sstream>
#include <exception>

void	Location::_errorJumpTable(std::vector<std::string>& line) // naam aanpassen
{
	const std::string server_elements[] = {"listen", "server_name", "error_page", "location"};

	if (isIn(line[0], server_elements, sizeof(server_elements)))
		throw MissingClosingBracket("Location");
	else
		throw DirectiveNotRecognized(line);
}

void	Location::_checkVarSet()
{
	if (this->_index.empty())
	{
		this->_index.push_back("index.html");
		this->_index.push_back("index");
	}
	if (this->_uploadStore.empty())
		this->_uploadStore = this->_root;
	//all standard set var setten en checken of er meer gezet moet worden
}

Location::Location(std::deque<std::string>& file, std::string& title) : _title(title), _root("/html"), _clientMaxBodySize(16000), _autoindex(false), _staticDir(false), _redir(Redir("0", ""))
{
	std::vector<std::string>	splitted;

	while (!file.empty())
	{
		splitted = split(file[0]);
		file.pop_front();
		if (splitted.size() == 0)
			continue ;
		if (splitted[0] == "}")
		{

			return ;
		}
		else if (splitted[0] == "root")
			this->setRoot(splitted);
		else if (splitted[0] == "client_max_body_size")
			this->setClientMaxBodySize(splitted);
		else if (splitted[0] == "index")
			this->setIndex(splitted);
		else if (splitted[0] == "autoindex")
			this->setAutoindex(splitted);
		else if (splitted[0] == "static_dir")
			this->setStaticDir(splitted);
		else if (splitted[0] == "cgi")
			this->addCgi(splitted);
		else if (splitted[0] == "limit_except")
			this->setLimitExcept(splitted);
		else if (splitted[0] == "upload_store")
			this->setUploadStore(splitted);
		else if (splitted[0] == "redir")
			this->setUploadStore(splitted);
		else
			this->_errorJumpTable(splitted);
	}
	throw MissingClosingBracket("Location");
}

Location&	Location::operator=(const Location& ref)
{
	this->_title = ref._title;
	this->_root = ref._root;
	this->_clientMaxBodySize = ref._clientMaxBodySize;
	this->_index = ref._index;
	this->_autoindex = ref._autoindex;
	this->_staticDir = ref._staticDir;
	this->_cgi = ref._cgi;
	this->_limitExcept = ref._limitExcept;
	this->_uploadStore = ref._uploadStore;
	return *this;
}

Location::Location(const Location& ref)
{
	*this = ref;
}

Location::Location(std::string) : _clientMaxBodySize(0), _autoindex(false), _staticDir(false)
{
	return ;
}

Location::~Location()
{
	return ;
}

/* -Setters- */

void	Location::setTitle(std::string& title)
{
	this->_title = title;
}

void	Location::setRoot(std::vector<std::string>& line)
{
	if (line.size() != 2)
		throw ArgumentIncorrect(line);
	this->_root = line[1];
}

void	Location::setClientMaxBodySize(std::vector<std::string>& line)
{
	unsigned long number;
	unsigned int multiplier;
	size_t idx;

	if (line.size() != 2)
		throw ArgumentIncorrect(line);
	multiplier = 1;
	idx = line[1].find_first_not_of("0123456789");
	if (idx != std::string::npos)
	{
		if (idx != line[1].length() - 1)
			throw cmbsUnitPrefix(line);
		switch(line[1][idx])
		{
			case 'k':
				multiplier = 1000;
				break ;
			case 'm':
				multiplier = 1000000;
				break ;
			case 'g':
				multiplier = 1000000000;
				break ;
			default:
				throw cmbsUnitPrefix(line); 
		}
	}
	std::istringstream (line[1]) >> number;
	this->_clientMaxBodySize = number * multiplier;
}

void	Location::setIndex(std::vector<std::string>& line)
{
	if (line.size() <= 1)
		throw ArgumentIncorrect(line);
	line.erase(line.begin());
	this->_index = line;
}

void	Location::setAutoindex(std::vector<std::string>& line)
{
	if (line.size() != 2)
		throw ArgumentIncorrect(line);
	if (line[1] == "on")
		this->_autoindex = true;
	else if (line[1] == "off")
		this->_autoindex = false;
	else
		throw ElemDefNotRecognized("Auto index", "[\"on\"/\"off\"]", line);
}

void	Location::setStaticDir(std::vector<std::string>& line)
{
	if (line.size() != 2)
		throw ArgumentIncorrect(line);
	if (line[1] == "true")
		this->_staticDir = true;
	else if (line[1] == "false")
		this->_staticDir = false;
	else
		throw ElemDefNotRecognized("Static dir", "[\"true\"/\"false\"]", line);
}

void	Location::addCgi(std::vector<std::string>& line)
{
	if (line.size() != 3)
		throw ArgumentIncorrect(line);
	this->_cgi[line[1]] = line[2];
}

bool	Location::isIn(const std::string& elem, const std::string *array, size_t size) const
{
	for (size_t idx = 0; idx < size/sizeof(std::string); idx++)
	{
		if (elem == array[idx])
			return true;
	}
	return false;
}

void	Location::setLimitExcept(std::vector<std::string>& line)
{
	const std::string correctMethods[] = {"GET", "POST", "DELETE"};

	if (line.size() <= 1)
		throw ArgumentIncorrect(line);
	for (std::vector<std::string>::iterator itr = line.begin() + 1; itr != line.end(); itr++)
		if (!this->isIn(*itr, correctMethods, sizeof(correctMethods)))
			throw leInvalidMethod(line, *itr);
	this->_limitExcept = std::set<std::string>(line.begin() + 1, line.end());
}

void	Location::setUploadStore(std::vector<std::string>& line)
{
	if (line.size() != 2)
		throw ArgumentIncorrect(line);
	this->_uploadStore = line[1];
}

void	Location::setRedir(std::vector<std::string>& line)
{
	if (line.size() != 3)
		throw ArgumentIncorrect(line);
	this->_redir = Redir(line[1], line[2]);
}

/* Getters */

const std::string&	Location::getTitle() const
{
	return this->_title;
}

const std::string&	Location::getRoot() const
{
	return this->_root;
}

unsigned long	Location::getClientMaxBodySize() const
{
	return this->_clientMaxBodySize;
}

const std::vector<std::string>&	Location::getIndex() const
{
	return this->_index;
}

bool	Location::getAutoindex() const
{
	return this->_autoindex;
}

bool	Location::getStaticDir() const
{
	return this->_staticDir;
}

const std::map<std::string, std::string>&	Location::getCgi() const
{
	return this->_cgi;
}

const std::set<std::string>&	Location::getLimitExcept() const
{
	return this->_limitExcept;
}

const std::string&	Location::getUploadStore() const
{
	return this->_uploadStore;
}

const Redir&	Location::getRedir() const
{
	return this->_redir;
}

std::vector<std::string>::const_iterator	Location::getRightIndexFile(const std::string prefix) const
{
	std::vector<std::string>::const_iterator itr = this->_index.begin();
	std::string filename;
	for (; itr != this->_index.end(); itr++)
	{
		filename = prefix + *itr;
		if (doesFileExist(filename))
			return (itr);
	}
	return (itr);
}

std::ostream&	operator<< (std::ostream& out, const Location& obj)
{
	out << std::boolalpha;
	out << "Location >-> title: " << obj.getTitle() << " -=-=-=-" << std::endl;
	out << "\troot = {" << obj.getRoot() << "}" << std::endl;
	out << "\tclient_max_body_size = [" << obj.getClientMaxBodySize() << "]" << std::endl;
	out << "\tindex =";
	std::vector<std::string> tempIndex = obj.getIndex();
	for (std::vector<std::string>::iterator it = tempIndex.begin(); it != tempIndex.end(); it++)
		out << " {" << *it << "}";
	out << std::endl;
	out << "\tautoindex = {" << obj.getAutoindex() << "}" << std::endl;
	out << "\tstatic_dir = {" << obj.getStaticDir() << "}" << std::endl;
	out << "\tcgi =";
	std::map<std::string, std::string> tempCgi = obj.getCgi();
	for (std::map<std::string, std::string>::iterator it = tempCgi.begin(); it != tempCgi.end(); it++)
		out << " {\"" << it->first << "\", \"" << it->second << "\"}";
	out << std::endl;
	out << "\tlimit_except =";
	std::set<std::string> tempLimitExcept = obj.getLimitExcept();
	for (std::set<std::string>::iterator it = tempLimitExcept.begin(); it != tempLimitExcept.end(); it++)
		out << " {" << *it << "}";
	out << std::endl;
	out << "\tupload_store = {" << obj.getUploadStore() << "}" << std::endl;
	out << "\t" << obj.getRedir();
	return out;
}
