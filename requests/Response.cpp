/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 11:57:45 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/15 17:45:17 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <Response.hpp>
#include <sstream>

Response::Response()
{
	std::cout << "default constructor called" << std::endl;
}

Response::Response(std::string error)
{
	std::cout << "an error ocurred in Response constructor" << std::endl;
}

Response::Response(std::string file, Server server)
{
	StatusCodes statusCodes;
	std::stringstream ss;

	this->_path = "files/" + file;
	setResponseBody(this->_path);
	this->_statusCode = statusCodes.getStatusCode(200);
	ss << "HTTP/1.1 " << this->_statusCode.first << ' ' << this->_statusCode.second << "\r\n";
	ss << "Server: " << *(server.getServerName().begin()) << "\r\n";
	/* HARDCODED ALERT */
	ss << "Date: Tuesday, 25-Nov-97 01:22:04 GMT" << "\r\n";
	ss << "Last-modified: Thursday, 20-Nov-97 10:44:53 GMT" << "\r\n";
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: text/html" << "\r\n";
	ss << "Connection: Keep-Alive" << "\r\n\r\n";
	ss << getResponseBody();
	this->_response = ss.str();
}

Response::Response(const Response& ref)
{
	*this = ref;
}

Response&	Response::operator=(const Response& ref)
{
	this->_path = ref._path;
	this->_statusCode = ref._statusCode;
	this->_response = ref.getResponse();
	return (*this);
}

Response::~Response() {}

const std::string	&Response::getPath() const
{
	return (this->_path);
}

const std::pair<int, std::string>	&Response::getStatusCode() const
{
	return (this->_statusCode);
}

const std::string					&Response::getResponse() const
{
	return (this->_response);
}

const std::string					&Response::getResponseBody() const
{
	return (this->_responseBody);
}

void		Response::setResponseBody(std::string &filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;

	std::cout << filename << std::endl;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			this->_responseBody.append(line);
		}
	}
	else
		throw NotAFile();
}

std::ostream&	operator<<(std::ostream &out, const Response &obj)
{
	out << "Response path:\n" << obj.getPath() << "\nStatus code:\n[" << obj.getStatusCode().first << "]" << std::endl;
	return (out);
}
