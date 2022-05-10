
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
	Type								_type;
	std::string							_input;
	std::string							_location;
	std::map<std::string, std::string>	_headers;
	bool								_isChunked;
	bool								_isFinished;
	std::string							_body;

	public:
	Request();	
	Request(const Request& ref);
	Request& operator=(const Request& ref);
	~Request();

	Type								const &getType() const;
	std::string 						const &getLocation() const;
	std::map<std::string, std::string>	const &getHeaders() const;
	std::string							const &getInput() const;
	std::string							const &getBody() const;

	void						setResponse(Response response);
	void						setRequest(void);
	void						setHeaders(void);
	void						addto_request(int fd);
	bool						isFinished(void);
	bool						checkIfChunked(void) const;
	bool						readyForParse(void) const;
	void						readChunked(int fd);

	private: /* -Exception- */
		class NotAFile : public std::exception
		{
			const char*	what (void) const throw()
			{
				return ("Can't open this file");
			}
		};

		class IncorrectHTTP : public std::exception
		{
			const char*	what (void) const throw()
			{
				return ("Incorrect HTTP version");
			}
		};
};

std::ostream&	operator<< (std::ostream& out, const Request& obj);

#endif
