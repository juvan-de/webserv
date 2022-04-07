/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   StatusCodes.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 12:09:08 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/06 13:38:28 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "StatusCodes.hpp"

StatusCodes::StatusCodes()
{
	this->_statusCodes.insert(std::pair<int, std::string>(100, "Continue"));
	this->_statusCodes.insert(std::pair<int, std::string>(101, "Switching Protocols"));
	this->_statusCodes.insert(std::pair<int, std::string>(200, "OK"));
	this->_statusCodes.insert(std::pair<int, std::string>(201, "Created"));
	this->_statusCodes.insert(std::pair<int, std::string>(202, "Accepted"));
	this->_statusCodes.insert(std::pair<int, std::string>(203, "Non-Authoritative Information"));
	this->_statusCodes.insert(std::pair<int, std::string>(204, "No Content"));
	this->_statusCodes.insert(std::pair<int, std::string>(205, "Reset Content"));
	this->_statusCodes.insert(std::pair<int, std::string>(206, "Partial Content"));
	this->_statusCodes.insert(std::pair<int, std::string>(300, "Multiple Choices"));
	this->_statusCodes.insert(std::pair<int, std::string>(301, "Moved Permanently"));
	this->_statusCodes.insert(std::pair<int, std::string>(302, "Found"));
	this->_statusCodes.insert(std::pair<int, std::string>(303, "See Other"));
	this->_statusCodes.insert(std::pair<int, std::string>(304, "Not Modified"));
	this->_statusCodes.insert(std::pair<int, std::string>(305, "Use Proxy"));
	this->_statusCodes.insert(std::pair<int, std::string>(307, "Temporary Redirect"));
	this->_statusCodes.insert(std::pair<int, std::string>(400, "Bad Request"));
	this->_statusCodes.insert(std::pair<int, std::string>(401, "Unauthorized"));
	this->_statusCodes.insert(std::pair<int, std::string>(402, "Payment Required"));
	this->_statusCodes.insert(std::pair<int, std::string>(403, "Forbidden"));
	this->_statusCodes.insert(std::pair<int, std::string>(404, "Not Found"));
	this->_statusCodes.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	this->_statusCodes.insert(std::pair<int, std::string>(406, "Not Acceptable"));
	this->_statusCodes.insert(std::pair<int, std::string>(407, "Proxy Authentication Required"));
	this->_statusCodes.insert(std::pair<int, std::string>(408, "Request Time-out"));
	this->_statusCodes.insert(std::pair<int, std::string>(409, "Conflict"));
	this->_statusCodes.insert(std::pair<int, std::string>(410, "Gone"));
	this->_statusCodes.insert(std::pair<int, std::string>(411, "Length Required"));
	this->_statusCodes.insert(std::pair<int, std::string>(412, "Precondition Failed"));
	this->_statusCodes.insert(std::pair<int, std::string>(413, "Request Entity Too Large"));
	this->_statusCodes.insert(std::pair<int, std::string>(414, "Request-URI Too Large"));
	this->_statusCodes.insert(std::pair<int, std::string>(415, "Unsupported MEdia Type"));
	this->_statusCodes.insert(std::pair<int, std::string>(416, "Requested range not satisfiable"));
	this->_statusCodes.insert(std::pair<int, std::string>(417, "Expectation Failed"));
	this->_statusCodes.insert(std::pair<int, std::string>(500, "Internal Server Error"));
	this->_statusCodes.insert(std::pair<int, std::string>(501, "Not Implemented"));
	this->_statusCodes.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	this->_statusCodes.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	this->_statusCodes.insert(std::pair<int, std::string>(504, "Gateway Time-out"));
	this->_statusCodes.insert(std::pair<int, std::string>(505, "HTTP Version not supported"));
}

StatusCodes::StatusCodes(const StatusCodes& ref)
{
	*this = ref;
}

StatusCodes&	StatusCodes::operator=(const StatusCodes& ref)
{
	this->_statusCodes = ref.getStatusCodes();
	return (*this);
}

StatusCodes::~StatusCodes()
{

}

const std::pair<int, std::string>			StatusCodes::getStatusCode(int key) const
{
	std::map<int, std::string>::const_iterator it = this->_statusCodes.find(key);
	if (it != _statusCodes.end())
		return (*it);
	else
		throw CodeNotFound();
}

const std::map<int, std::string>&	StatusCodes::getStatusCodes() const
{
	return (this->_statusCodes);
}
