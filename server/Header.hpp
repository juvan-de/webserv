/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/01 17:38:37 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <iostream>

enum Type 
{
	GET,
	POST,
	DELETE
};

class	Header
{
	private:
	Type		_type;
	bool		_connection;
	std::string	_host;
	int			_port;
	
	public:

	Header(std::string request);
	Header(const Header& ref);
	Header& operator=(const Header& ref);
	~Header();
};

#endif
