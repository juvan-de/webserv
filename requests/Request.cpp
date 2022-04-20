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
	this->_isChunked = false;
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

std::string		const &Request::getInput() const
{
	return (this->_input);
}

std::string		const &Request::getBody() const
{
	return (this->_body);
}

void			Request::addto_request(int fd)
{
	char	cstr[BUFFER_SIZE + 1];
	int		ret = 1;

	// this can return an error if operation would block, see man page
	ret = recv(fd, cstr, BUFFER_SIZE, MSG_DONTWAIT);
	if (ret > 0)
	{
		cstr[ret] = '\0';
		this->_input.append(cstr);
		std::cout << "*********input*********\n" << this->_input << "\n*********input*********" << "\nret: " << ret << std::endl;
	}
	else if (ret <= -1)
		std::cout << "\033[31m" << "RECV ERROR: " << ret << "\033[0m" << std::endl;
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
	if (first_line.find("HTTP/1.1") == std::string::npos)
		throw IncorrectHTTP();
} 

void		Request::setHeaders(void)
{
	this->_input = this->_input.substr(this->_input.find('\n') + 1);
	size_t end = this->_input.find("\r\n\r\n") + 4;
	std::string headers = this->_input.substr(0, end);
	std::vector<std::string> lines = split(headers, "\r\n");
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (it->find(":") != std::string::npos)
		{
			std::string first = it->substr(0, it->find(':'));
			std::string second = it->substr(it->find(':') + 1);
			second = strtrim(second, " \r\n");
			if (first.size() > 1)
				this->_headers[first] = second;
		}
	}
	this->_input = this->_input.substr(end);
	// for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
	// {
	// 	std::cout << "first: (" << it->first << ")\tsecond: (" << it->second << ")" << std::endl;
	// }
	std::map<std::string, std::string>::iterator it = this->_headers.find("Transfer-Encoding");
	if (it != this->_headers.end() && it->second == "chunked")
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

bool		Request::checkIfChunked(void) const
{
	return (this->_isChunked);
}

bool		Request::readyForParse(void) const
{
	return (this->_isFinished);
}

void			Request::readChunked(int fd)
{
	int bodysize = std::stoi(this->_input, NULL, 16);
	this->_body.append(this->_input.substr(this->_input.find("\r\n") + 2, bodysize));
	this->_input = this->_input.substr(this->_input.find("\r\n") + 4 + bodysize);
	if (this->_input == "0\r\n\r\n")
	{
		this->_isFinished = true;
		std::cout << "we finishin\n";
	}
}

std::ostream&	operator<< (std::ostream& out, const Request& obj)
{
	out << "Input data:\n" << obj.getInput() << "\nEnd Input" << std::endl;
	if (obj.checkIfChunked())
		out << "The request is chunked" << std::endl;
	if (obj.readyForParse())
		out << "The request is fully read" << std::endl;
	out << "location: " << obj.getLocation() << std::endl;
	out << "HEADERS: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = obj.getHeaders().begin(); it != obj.getHeaders().end(); it++)
	{
		std::cout << "first: (" << it->first << ")\tsecond: (" << it->second << ")" << std::endl;
	}
	out << "BODY: \n" << obj.getBody();
	return (out);
}