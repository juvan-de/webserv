/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 11:57:48 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/02 17:01:04 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <StatusCodes.hpp>
# include <Server.hpp>

class	Response
{
	private:

	std::string					_statusLine;
	std::pair<int, std::string>	_statusCode;
	std::string					_path;

	public:

	Response();
	Response(std::string error);
	Response(std::string path, Server server);
	Response(const Response& ref);
	Response& operator=(const Response& ref);
	~Response();

	const std::string	&getPath() const;
	const std::pair<int, std::string>	&getStatusCode() const;
	const std::string					&getStatusLine() const;
};

std::ostream&	operator<< (std::ostream& out, const Response& obj);

#endif
