#include <Request.hpp>
#include <fstream>

Request::Request()
{
	std::vector<std::string> empty;
	this->_Requests = empty;
	this->_type = ERROR;
	this->_location = "";
}

Request::Request(std::string request, int clisock)
{
	this->_clisock = clisock;
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
	size_t end = request.find(' ');
	this->_location = request.substr(start, end);
	/* I skip the HTTP/1.1 here*/
	end = request.find('\n');
	start = end + 1;
	while (start < request.size() && end != std::string::npos)
	{
		end = request.find('\n', start);
		this->_Requests.push_back(request.substr(start, end - start - 1));
		start = end + 1;
	}
}

Request::Request(const Request& ref)
{
	*this = ref;
}

Request&	Request::operator=(const Request& ref)
{
	this->_type = ref.getType();
	this->_location = ref.getLocation();
	this->_Requests = ref.getRequests();
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

std::vector<std::string>	const &Request::getRequests() const
{
	return (this->_Requests);
}

Response	const &Request::getResponse() const
{
	return (this->_response);
}

int			const &Request::getClisock() const
{
	return (this->_clisock);
}

void			Request::setResponse(Response response)
{
	this->_response = response;
}