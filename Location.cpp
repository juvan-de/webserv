#include <deque>
#include "Location.hpp"

typedef void (Location::*fptr)(std::vector<std::string>&);

fptr	Location::switchcase(std::string)
{
	switch(tokens[0])
	{
		case "}":
			NULL;
		case "root":
			return &this->setRoot;
		case "client_max_body_size":
			return &this->setMaxBodySize;
		case "index":
			return &this->setIndex;
		case "autoindex":
			return &this->setAutoIndex;
		case "static_dir":
			return &this->setStaticDir;
		case "cgi":
			return &this->setCgi;
		case "limit_except":
			return &this->setLimitExcept;
		case "upload_store":
			return &this->setUploadStore;
		case "redir":
			return &this->setUploadStore;
		default:
			throw("aanpassen");
	}
}

template<class T>
void	parseLoop(T& block, std::deque<std::string>& config)
{
	while (!config.empty())
	{
		std::string& line = config.front();
		config.pop_front();
		std::vector<std::string> splitted = split(line);
		fptr setfunc = block.switchcase(splitted[0]);
		if (setfunc == NULL)
			return ;
		block.(*setfunc)(splitted);
	}
}

Location::Location(std::deque<std::string>& config)
{
	parseLoop(this, config);
}

Location::Location(std::vector<std::string> loc)
{
	for (size_t i = 0; i < loc.size(); i++)
	{
		switch()
			case "}":
				return ;
			case "root":
				loc.setRoot();
				break ;
			case "client_max_body_size":
				loc.setMaxBodySize();
				break ;
			case "index":
				loc.setIndex();
				break ;
			case "autoindex":
				loc.setAutoIndex();
				break ;
			case "static_dir":
				loc.setStaticDir();
				break ;
			case "cgi":
				loc.setCgi();
				break ;
			case "limit_except":
				loc.setLimitExcept();
				break ;
			case "upload_store":
				loc.setUploadStore();
				break ;
			case "redir":
				loc.setUploadStore();
				break ;
			default:
				trow();
	}
}

Location&	Location::operator=(const Location& ref)
{
	this->_title = ref._title
	this->_root = ref._root;
	this->_clientMaxBodySize = ref._clientMaxBodySize;
	this->_index = ref._index;
	this->_autoindex = ref._autoindex;
	this->_static_dir = ref._static_dir;
	this->_cgi = ref._cgi;
	this->_limitExcept = ref._limitExcept;
	this->_uploadStore = ref._uploadStore;
	this->root = ref.root;
	return *this;
}

Location::Location(const Location& ref)
{
	*this = ref;
}

Location::~Location()
{
	return ;
}

std::ostream&	operator<< (std::ostream& out, const Redir& obj)
{
	out << std::boolalpha;
	out << "-=- Location > " << obj.getTitle() << " -=-=-=-" << std::endl;
	out << "root={" << obj.getRoot() << "}" << std::endl;
	out << "client_max_body_size={" << obj.getClientMaxBodySize() << "}" << std::endl;
	out << "index={" << obj.getIndex() << "}" << std::endl;
	out << "autoindex={" << obj.getAutoIndex() << "}" << std::endl;
	out << "static_dir={" << obj.getStaticDir() << "}" << std::endl;
	out << "cgi=";
	std::map<std::string, std::string> temp = obj.getcgi();
	for (std::map<std::string, std::string>::iterator it = temp.begin(); it != temp.end(); it++)
		out << " {\"" << it.first << "\", \"" << it.last << "}";
	out << "limit_except=";
	std::set<std::string> temp = obj.getLimitExcept();
	for (std::set<std::string>::iterator it = temp.begin(); it != tem.end(); it++)
		out << " {" << *it << "}";
	out << obj.getRedir() << std::endl;
	return out;
}