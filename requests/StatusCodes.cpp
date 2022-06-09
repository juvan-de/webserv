/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   StatusCodes.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 12:09:08 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/06/09 13:26:02 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "StatusCodes.hpp"
#include <iostream>

StatusCodes::StatusCodes()
{
	this->_statusCodes[100] = "Continue";
	this->_statusCodes[101] = "Switching Protocols";
	this->_statusCodes[200] = "OK";
	this->_statusCodes[201] = "Created";
	this->_statusCodes[202] = "Accepted";
	this->_statusCodes[203] = "Non-Authoritative Information";
	this->_statusCodes[204] = "No Content";
	this->_statusCodes[205] = "Reset Content";
	this->_statusCodes[206] = "Partial Content";
	this->_statusCodes[300] = "Multiple Choices";
	this->_statusCodes[301] = "Moved Permanently";
	this->_statusCodes[302] = "Found";
	this->_statusCodes[303] = "See Other";
	this->_statusCodes[304] = "Not Modified";
	this->_statusCodes[305] = "Use Proxy";
	this->_statusCodes[307] = "Temporary Redirect";
	this->_statusCodes[400] = "Bad Request";
	this->_statusCodes[401] = "Unauthorized";
	this->_statusCodes[402] = "Payment Required";
	this->_statusCodes[403] = "Forbidden";
	this->_statusCodes[404] = "Not Found";
	this->_statusCodes[405] = "Method Not Allowed";
	this->_statusCodes[406] = "Not Acceptable";
	this->_statusCodes[407] = "Proxy Authentication Required";
	this->_statusCodes[408] = "Request Time-out";
	this->_statusCodes[409] = "Conflict";
	this->_statusCodes[410] = "Gone";
	this->_statusCodes[411] = "Length Required";
	this->_statusCodes[412] = "Precondition Failed";
	this->_statusCodes[413] = "Request Entity Too Large";
	this->_statusCodes[414] = "Request-URI Too Large";
	this->_statusCodes[415] = "Unsupported Media Type";
	this->_statusCodes[416] = "Requested range not satisfiable";
	this->_statusCodes[417] = "Expectation Failed";
	this->_statusCodes[500] = "Internal Server Error";
	this->_statusCodes[501] = "Not Implemented";
	this->_statusCodes[502] = "Bad Gateway";
	this->_statusCodes[503] = "Service Unavailable";
	this->_statusCodes[504] = "Gateway Time-out";
	this->_statusCodes[505] = "HTTP Version not supported";
}

const std::string&	StatusCodes::IStatusCode(int key)
{
	return this->_statusCodes.find(key)->second;
}
