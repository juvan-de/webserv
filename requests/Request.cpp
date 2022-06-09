#include <unistd.h>
#include <limits.h>

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
	this->_statusCode = 200;
	this->_bytesRead = 0;
}

Request::Request(const Request& ref)
{
	*this = ref;
}

Request&	Request::operator=(const Request& ref)
{
	this->_type = ref.getType();
	this->_uri = ref.getUri();
	this->_headers = ref.getHeaders();
	this->_input = ref.getInput();
	this->_isChunked = ref.checkIfChunked();
	this->_isFinished = ref.readyForParse();
	this->_body = ref.getBody();
	this->_statusCode = ref.getStatusCode();
	this->_bytesRead = ref.getBytesRead();
	return (*this);
}

Request::~Request() {}

const Type&		Request::getType() const 
{
	return (this->_type);
}

const std::string&	Request::getUri() const 
{
	return (this->_uri);
}

const std::map<std::string, std::string, cmpCaseInsensitive >&	Request::getHeaders() const
{
	return (this->_headers);
}

const std::string&	Request::getInput() const
{
	return (this->_input);
}

const std::string&	Request::getBody() const
{
	return (this->_body);
}

int	Request::getStatusCode() const
{
	return (this->_statusCode);
}

size_t Request::getBytesRead() const
{
	return (this->_bytesRead);
}

void			Request::addto_request(int fd)
{
	char	cstr[BUFFER_SIZE];
	int		ret = 1;

	// this can return an error if operation would block, see man page
	ret = recv(fd, cstr, BUFFER_SIZE, 0);
	if (ret > 0 && ret < BUFFER_SIZE)
	{
		this->_input.append(cstr, ret);
		this->_bytesRead = ret;
		if (this->_type != NOTSET)
			this->_isFinished = true;
	}
	else if (ret > 0)
	{
		this->_input.append(cstr, ret);
		this->_bytesRead = ret;
	}
	else if (ret <= -1)
		throw RequestException(505);
}

bool			Request::isFinished(void)
{
	std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
	if (it == this->_headers.end()) //needs work
			return (true);
	else if(this->_body.size() >= 4 && this->_body.compare(this->_body.size() - 4, 4, "\r\n\r\n"))
	{
		size_t length = std::atoi(it->second.c_str());
		if (length == this->_body.size())
			return (true);
		return (false);
	}
	return (false);
}

void			Request::setRequest(void)
{
	std::string first_line (this->_input.substr(0, this->_input.find('\n')));
	std::vector<std::string> array = split_on_chars(first_line);
	if (array.size() != 3)
		throw RequestException(400);
	if (array[0] == "GET")
		this->_type = GET;
	else if (array[0] == "POST")
		this->_type = POST;
	else if (array[0] == "DELETE")
		this->_type = DELETE;
	else
		throw RequestException(400);
	this->_uri = array[1];
	if (array[2] != "HTTP/1.1")
		throw RequestException(505);
} 

void		Request::setHeaders(void)
{
	std::cout << "INPUT:\n" << _input << "\nendinput." << std::endl;
	size_t end = this->_input.find("\r\n\r\n") + 4;
	std::string headers = this->_input.substr(0, end);
	std::vector<std::string> lines = split_on_str(headers, "\r\n");
	lines.erase(lines.begin());
	for (size_t i = 0; i < lines.size(); i++)
	{
		if (lines[i].find(":") != std::string::npos)
		{
			std::vector<std::string> splitted = split_on_str(lines[i], ": ");
			this->_headers[splitted[0]] = splitted[1];
		}
	}
	this->_input = this->_input.substr(end);
	std::map<std::string, std::string>::iterator it = this->_headers.find("Transfer-Encoding");
	if (it != this->_headers.end() && it->second == "chunked")
	{
		this->_isChunked = true;
		this->_isFinished = false;
	}
	else
	{
		this->_isChunked = false;
		it = this->_headers.find("Content-Length");
		if (it == this->_headers.end() && this->_type == POST)
			throw RequestException(411);
		if (this->isFinished()) //unsure if failproof
			this->_isFinished = true;
	}
}

void		Request::append_body()
{
	for (size_t i = 0; i < this->_input.size(); i++)
		this->_body += this->_input[i];
	this->_input.clear();
	if (this->isFinished())
		this->_isFinished = true;
}

void		Request::setType(Type code)
{
	this->_type = code;
}

void		Request::setStatusCode(int code)
{
	this->_statusCode = code;
}

void		Request::setAsFinished()
{
	this->_isFinished = true;
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
	(void)fd;
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
	out << "Unparsed input data:\n" << obj.getInput() << "\nEnd Input" << std::endl;
	out << "TYPE: " << obj.getType() << std::endl;
	if (obj.getType() == 3)
		out << "statusCode: " << obj.getStatusCode() << std::endl;
	if (obj.checkIfChunked())
		out << "The request is chunked" << std::endl;
	if (obj.readyForParse())
	{
		out << "request fully read, body size: " << obj.getBody().size() << " and content length: " << obj.getHeaders().find("Content-Length")->second << std::endl;
	}
	out << "location: " << obj.getUri() << std::endl;
	out << "HEADERS: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = obj.getHeaders().begin(); it != obj.getHeaders().end(); it++)
	{
		out << "first: (" << it->first << ")\tsecond: (" << it->second << ")" << std::endl;
	}
	// out << "BODY:\n" << obj.getBody();
	return (out);
}