/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 11:57:45 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/02 17:12:10 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <sstream>
#include <Server.hpp>
#include <StatusCodes.hpp>

Response::Response()
{
	std::cout << "default constructor called" << std::endl;
}

Response::Response(std::string error)
{
	std::cout << "an error ocurred in Response constructor" << std::endl;
}

Response::Response(std::string path, Server server) : _path(path)
{
	StatusCodes statusCodes;
	std::stringstream ss;

	this->_statusCode = statusCodes.getStatusCode(200);
}

Response::Response(const Response& ref)
{
	*this = ref;
}

Response&	Response::operator=(const Response& ref)
{
	this->_path = ref.getPath();
	this->_statusCode = ref.getStatusCode();
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

std::ostream&	operator<<(std::ostream &out, const Response &obj)
{
	out << "Response path:\n" << obj.getPath() << "\nStatus code:\n[" << obj.getStatusCode().first << "]" << std::endl;
	return (out);
}