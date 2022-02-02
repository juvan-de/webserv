#include "Header.hpp"

Header::Header(std::string request)
{
	std::cout << request << std::endl;
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

	while (start < request.size())
	{
		end = request.find('\n', start);
		this->_headers.push_back(request.substr(start, end - start - 1));
		start = end + 1;
	}
	for (int i = 0; i < this->_headers.size(); i++)
	{
		std::cout << "header: [" << this->_headers[i] << "]" << std::endl;
	}
}

Header::Header(const Header& ref)
{
	*this = ref;
}

Header&	Header::operator=(const Header& ref)
{

}

Header::~Header()
{

}

int			Header::getType() const 
{
	return (this->_type);
}

std::string	Header::getPath() const 
{
	return (this->_path);
}