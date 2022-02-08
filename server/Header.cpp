#include "Header.hpp"

Header::Header()
{
	std::vector<std::string> empty;
	this->_headers = empty;
	this->_type = ERROR;
	this->_path = "";
}

Header::Header(std::string request)
{

	size_t size = request.find(' ');
	switch(size)
	{
		case 3 :	this->_type = GET;
					request = request.substr(4);
					break ;
		case 4 : 	this->_type = POST;
					request = request.substr(5);
					break;
		case 6 : 	this->_type = DELETE;
					request = request.substr(6);
					break;
	}
	size_t start = request.find('/');
	size_t end = request.find('\n');
	this->_path = request.substr(start, end);
	start = end + 1;
	while (start < request.size() && end != std::string::npos)
	{
		end = request.find('\n', start);
		this->_headers.push_back(request.substr(start, end - start - 1));
		start = end + 1;
	}
}

Header::Header(const Header& ref)
{
	*this = ref;
}

Header&	Header::operator=(const Header& ref)
{
	this->_type = ref.getType();
	this->_path = ref.getPath();
	this->_headers = ref.getHeaders();
	return (*this);
}

Header::~Header()
{
	
}

Type		Header::getType() const 
{
	return (this->_type);
}

std::string	Header::getPath() const 
{
	return (this->_path);
}

std::vector<std::string>	Header::getHeaders() const
{
	return (this->_headers);
}

std::string	Header::getResponse() const
{
	return (this->_response);
}

void		Header::setResponse(std::string response)
{
	this->_response = response;
}