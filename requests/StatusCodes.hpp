/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   StatusCodes.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/02 12:09:10 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/04/12 13:13:37 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_CODES_HPP
# define STATUS_CODES_HPP

# include <string>
# include <iostream>
# include <map>

class	StatusCodes
{
	private:
	std::map<int, std::string>	_statusCodes;

	public:

	StatusCodes();
	StatusCodes(const StatusCodes& ref);
	StatusCodes& operator=(const StatusCodes& ref);
	~StatusCodes();
	
	const std::pair<int, std::string> getStatusCode(int key) const;
	const std::map<int, std::string>&	getStatusCodes() const;

	class CodeNotFound : public std::exception
		{
			const char*	what(void) const throw()
			{
				return ("This StatusCode doesn't exist");
			};
		};
};

#endif
