#include <redir.hpp>
#include <exception>
#include <string>
#include <sstream>

Redir::Redir(std::vector<std::string>& line, std::string statuscode, std::string location)
{
	this->_isSet = true;
	this->setStatusCode(statuscode, line);
	this->setLocation(location);
}

Redir&	Redir::operator= (const Redir& ref)
{
	this->_isSet = ref._isSet;
	this->_statusCode = ref._statusCode;
	this->_location = ref._location;
	return *this;
}

std::ostream&	operator<< (std::ostream& out, const Redir& obj)
{
	out << "Redir = {" << obj.getLocation() << "} [" << obj.getStatusCode() << "]";
	return out;
}

Redir::Redir(const Redir& ref)
{
	*this = ref;
}

Redir::Redir() : _isSet(false)
{
	this->_isSet = false;
	return ;
}

Redir::~Redir()
{
	return ;
}

void	Redir::setStatusCode (const std::string& statusCode, const std::vector<std::string>& line)
{
	if (statusCode.find_first_not_of("0123456789") != std::string::npos)
		throw ElemNotANumber(statusCode, line);
	std::istringstream (statusCode) >> this->_statusCode;
	if (this->_statusCode != 301)
		throw RedirWrongStatusCode(this->_statusCode);
}

void	Redir::setStatusCode (unsigned int statusCode)
{
	this->_statusCode = statusCode;
}

void	Redir::setLocation (std::string location)
{
	this->_location = location;
}

unsigned int	Redir::getStatusCode (void) const
{
	return this->_statusCode;
}

std::string	Redir::getLocation (void) const
{
	return this->_location;
}

bool	Redir::isSet() const
{
	return this->_isSet;
}
