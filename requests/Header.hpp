/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Header.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/02/22 15:53:04 by juvan-de      ########   odam.nl         */
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
	std::string					_location;
	std::vector<std::string>	_headers;
	std::string					_response;
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
	std::string					const &getResponse() const;
	int							const &getClisock() const;
	void						setResponse(std::string &response);

	private: /* -Exception- */
		class NotAFile : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Can't open this file";
			}
		};
};


#endif
