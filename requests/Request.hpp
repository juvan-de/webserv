
#ifndef Request_HPP
# define Request_HPP

# include <string>
# include <iostream>
# include <vector>
# include <Response.hpp>
# include <utils.hpp>

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
	Type													_type;
	std::string												_input;
	std::string												_uri;
	std::map<std::string, std::string, cmpCaseInsensitive>	_headers;
	bool													_isChunked;
	bool													_isFinished;
	std::string												_body;
	int														_statusCode;
	size_t													_bytesRead;
	size_t													_bodySize;

	public:
	Request();	
	Request(const Request& ref);
	Request& operator=(const Request& ref);
	~Request();

	const Type&														getType() const;
	const std::string& 												getUri() const;
	const std::map<std::string, std::string, cmpCaseInsensitive>&	getHeaders() const;
	const std::string&												getInput() const;
	const std::string&												getBody() const;
	int																getStatusCode() const;
	size_t															getBytesRead() const;
	size_t															getBodySize() const;

	void						setResponse(Response response);
	void						setRequest(void);
	void						setHeaders(void);
	void						append_body(void);
	void						setType(Type code);
	void						setStatusCode(int code);
	void						setAsFinished();
	void						addto_request(int fd);
	bool						isFinished(void);
	bool						checkIfChunked(void) const;
	const std::string&			getRealPath(void) const;
	bool						readyForParse(void) const;
	void						readChunked(int fd);

	public: /* -Exception- */
		class RequestException : public std::exception 
		{
			private:
				int _statusCode;
			public:
			RequestException(int code) : _statusCode(code){}
			int	getError(void) const throw()
			{
				return (this->_statusCode);
			}			
		};
};

std::ostream&	operator<< (std::ostream& out, const Request& obj);

#endif
