/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/02 15:47:16 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <iostream>
# include <vector>

enum Type 
{
	GET,
	POST,
	DELETE
};

class	Header
{
	private:
	Type						_type;
	std::string					_path;
	std::vector<std::string>	_headers;
	
	public:

	Header(std::string request);
	Header(const Header& ref);
	Header& operator=(const Header& ref);
	~Header();

	int	getType() const;
	std::string getPath() const;
};

#endif
