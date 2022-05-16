
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
	int									_statusCode;

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
	int									const &getStatusCode() const;

	void						setResponse(Response response);
	void						setRequest(void);
	void						setHeaders(void);
	void						setType(Type code);
	void						setStatusCode(int code);
	void						setAsFinished();
	void						addto_request(int fd);
	bool						isFinished(void);
	bool						checkIfChunked(void) const;
	bool						readyForParse(void) const;
	void						readChunked(int fd);
	public: /* -Exception- */
		class RequestException : public std::exception 
		{
			private:
				int _statusCode;
			public:
			RequestException(int code) : _statusCode(code){}
			const int	getError(void) const throw()
			{
				return (this->_statusCode);
			}			
		};
};

std::ostream&	operator<< (std::ostream& out, const Request& obj);

#endif
