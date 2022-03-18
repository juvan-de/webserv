/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: juvan-de <juvan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 17:00:22 by juvan-de      #+#    #+#                 */
/*   Updated: 2022/03/15 16:52:36 by juvan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Request_HPP
# define Request_HPP

# include <string>
# include <iostream>
# include <vector>
# include <Response.hpp>

enum Type 
{
	GET,
	POST,
	DELETE,
	ERROR,
	NOTSET
};

class	Request
{
	private:
	Type						_type;
	std::string					_input;
	std::string					_location;
	std::vector<std::string>	_headers;
	Response					_response;
	
	public:
	Request();	
	Request(const Request& ref);
	Request& operator=(const Request& ref);
	~Request();

	Type						const &getType() const;
	std::string 				const &getLocation() const;
	std::vector<std::string>	const &getHeaders() const;
	Response					const &getResponse() const;
	void						setResponse(Response response);
	void						addto_request(int fd);
	bool						isFinished(void);
	void						setRequest(void);

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
