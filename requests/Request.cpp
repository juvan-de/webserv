#include <Request.hpp>
#include <fstream>
#include <sys/socket.h>
#include <utils.hpp>

/*temp */
#define BUFFER_SIZE 2000

Request::Request()
{
	this->_type = NOTSET;
	this->_isFinished = false;
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

std::map<std::string, std::string>	const &Request::getHeaders() const
{
	return (this->_headers);
}

Response	const &Request::getResponse() const
{
	return (this->_response);
}

std::string		const &Request::getInput() const
{
	return (this->_input);
}

void			Request::setResponse(Response response)
{
	this->_response = response;
}

void			Request::addto_request(int fd)
{
	char	cstr[BUFFER_SIZE + 1];
	int		ret = 1;

	// this can return an error if operation would block, see man page
	while ((ret = recv(fd, cstr, BUFFER_SIZE, MSG_DONTWAIT)) > 0)
	{
		cstr[ret] = '\0';
		this->_input.append(cstr);
	}
	if (ret < -1)
		std::cout << "\033[31m" << "RECV ERROR: " << ret << "\033[0m" << std::endl;
	std::cout << "*********input*********\n" << this->_input << "\n*********input*********" << "\nret: " << ret << std::endl;
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
	std::cout << "SET_REQUEST" << std::endl;
	std::string first_line (this->_input.substr(0, this->_input.find('\n')));
	size_t size = first_line.find(' ');
	switch(size)
	{
		case 3 :	this->_type = GET;
					break ;
		case 4 : 	this->_type = POST;
					break;
		case 6 : 	this->_type = DELETE;
					break;
	}
	size_t start = first_line.find('/');
	size_t end = first_line.find(' ', start);
	this->_location = first_line.substr(start, end - start);
	std::cout << first_line.substr(end + 1) << std::endl;
	if (first_line.find("HTTP/1.1") == std::string::npos)
		throw IncorrectHTTP();
} 

void		Request::setHeaders(void)
{
	std::string headers = this->_input.substr(this->_input.find('\n'));
//	std::cout << "START" << std::endl;
	std::vector<std::string> lines = split(headers, "\n");
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		std::string first = it->substr(0, it->find(':'));
		std::string second = it->substr(it->find(':') + 2);
		if (first.size() > 1)
			this->_headers[first] = second;
	}	
	// for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
	// {
	// 	std::cout << "first: " << it->first << "\tsecond: " << it->second << std::endl;
	// }
	// std::cout << "end" << std::endl;
}

void		Request::checkIfChunked(void)
{
	if (this->_headers["Transfer-Encoding"] == "chunked")
	{
		this->_isChunked = true;
		this->_isFinished = false;
	}
	else
	{
		this->_isChunked = false;
		this->_isFinished = true;
	}
}

bool		Request::readyForParse(void) const
{
	return (this->_isFinished);
}