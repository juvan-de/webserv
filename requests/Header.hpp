/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/09 16:33:33 by juvan-de      ########   odam.nl         */
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
	DELETE,
	ERROR
};

class	Header
{
	private:
	Type						_type;
	std::string					_path;
	std::vector<std::string>	_headers;
	std::string					_response;
	int							_clisock;
	
	public:
	Header();	
	Header(std::string request, int clisock);
	Header(const Header& ref);
	Header& operator=(const Header& ref);
	~Header();

	Type						getType() const;
	std::string 				getPath() const;
	std::vector<std::string>	getHeaders() const;
	std::string					getResponse() const;
	int							getClisock() const;
	void						setResponse(std::string &response);
};


#endif
