/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/15 16:52:36 by juvan-de      ########   odam.nl         */
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
	std::string					_location;
	std::vector<std::string>	_headers;
	int							_clisock;
	Response					_response;
	
	public:
	Header();	
	Header(std::string request, int clisock);
	Header(const Header& ref);
	Header& operator=(const Header& ref);
	~Header();

	Type						const &getType() const;
	std::string 				const &getLocation() const;
	std::vector<std::string>	const &getHeaders() const;
	Response					const &getResponse() const;
	int							const &getClisock() const;
	void						setResponse(Response response);

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
