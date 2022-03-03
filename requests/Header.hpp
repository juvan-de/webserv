/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/02 16:51:55 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <Response.hpp>

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
	Response					_response;
	std::string					_location;
	std::vector<std::string>	_headers;
	std::string					_responseStr;
	int							_clisock;
	
	public:
	Header();	
	Header(std::string request, int clisock);
	Header(const Header& ref);
	Header& operator=(const Header& ref);
	~Header();

	Type						const &getType() const;
	std::string 				const &getLocation() const;
	std::vector<std::string>	const &getHeaders() const;
	std::string					const &getResponseStr() const;
	int							const &getClisock() const;
	void						setResponse(Response response);
	void						setResponseBody(std::string &response);

	private: /* -Exception- */
		class NotAFile : public std::exception
		{
			const char*	what (void) const throw()
			{
				return ("Can't open this file");
			}
		};
};


#endif
