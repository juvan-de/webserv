#include <redir.hpp>
#include <exception>
#include <string>
#include <sstream>

Redir::Redir(std::string statuscode, std::string location)
{
	this->setStatusCode(statuscode);
	this->setLocation(location);
}

Redir&	Redir::operator= (const Redir& ref)
{
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

Redir::~Redir()
{
	return ;
}

void	Redir::setStatusCode (std::string statusCode)
{
	if (statusCode.find_first_not_of("0123456789") != std::string::npos)
		throw NotANumber();
	std::istringstream (statusCode) >> this->_statusCode;
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
