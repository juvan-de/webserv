#include <Header.hpp>
#include <fstream>

Header::Header()
{
	std::vector<std::string> empty;
	this->_headers = empty;
	this->_type = ERROR;
	this->_location = "";
}

Header::Header(std::string request, int clisock)
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
	this->_location = ref.getLocation();
	this->_headers = ref.getHeaders();
	this->_responseStr = ref.getResponseStr();
	return (*this);
}

Header::~Header() {}

Type		const &Header::getType() const 
{
	return (this->_type);
}

std::string	const &Header::getLocation() const 
{
	return (this->_location);
}

std::vector<std::string>	const &Header::getHeaders() const
{
	return (this->_headers);
}

std::string	const &Header::getResponseStr() const
{
	return (this->_responseStr);
}

int			const &Header::getClisock() const
{
	return (this->_clisock);
}

// void		Header::wrapResponse()
// {
// 	Response	response();
// 	StatusCodes	codes();

// }

void		Header::setResponseBody(std::string &filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			this->_responseStr.append(line).append("\n");
		}
	}
	else
		throw NotAFile();
	file.close();
}

void		Header::setResponse(Response response)
{
	this->_response = response;
}