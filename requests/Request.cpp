#include <Request.hpp>
#include <fstream>
#include <unistd.h>

/*temp */
#define BUFFER_SIZE 2000

Request::Request()
{
	this->_type = NOTSET;

}

Request::Request(const Request& ref)
{
	*this = ref;
}

Request&	Request::operator=(const Request& ref)
{
	this->_type = ref.getType();
	this->_location = ref.getLocation();
	this->_headers = ref.getHeaders();
	return (*this);
}

Request::~Request() {}

Type		const &Request::getType() const 
{
	return (this->_type);
}

std::string	const &Request::getLocation() const 
{
	return (this->_location);
}

std::vector<std::string>	const &Request::getHeaders() const
{
	return (this->_headers);
}

Response	const &Request::getResponse() const
{
	return (this->_response);
}

void			Request::setResponse(Response response)
{
	this->_response = response;
}

void			Request::addto_request(int fd)
{
	char	*cstr = new char[BUFFER_SIZE + 1];
	int		ret;
	ret = read(fd, cstr, BUFFER_SIZE);
	if (ret > 0)
		this->_input.append(cstr);
}

bool			Request::isFinished(void)
{
	std::string::reverse_iterator rit = this->_input.rbegin();
	if (rit[0] == '\n' && rit[1] == '\r' && rit[2] == '\n' && rit[3] == '\r')
		return (true);
	return (false);
}

void			Request::setRequest(void)
{
	size_t size = this->_input.find(' ');
	switch(size)
	{
		case 3 :	this->_type = GET;
					this->_input = this->_input.substr(4);
					break ;
		case 4 : 	this->_type = POST;
					this->_input = this->_input.substr(5);
					break;
		case 6 : 	this->_type = DELETE;
					this->_input = this->_input.substr(6);
					break;
	}
	size_t start = this->_input.find('/');
	size_t end = this->_input.find(' ');
	this->_location = this->_input.substr(start, end);
	/* I skip the HTTP/1.1 here*/
	end = this->_input.find('\n');
	start = end + 1;
	while (start < this->_input.size() && end != std::string::npos)
	{
		end = this->_input.find('\n', start);
		this->_headers.push_back(this->_input.substr(start, end - start - 1));
		start = end + 1;
	}	
}