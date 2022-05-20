
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <StatusCodes.hpp>
# include <ContentTypes.hpp>
# include <Server.hpp>

class	Response
{
	private:
		std::string							_response;
		std::string							_responseBody;
		bool								_isFinished;

		void	_setContentTypes();
		void	_makeDefaultErrorPage(int errorCode, const std::string& errorStatus);

	public:

	Response();
	Response(const Server *server, const std::string path); //valid
	Response(int errorcode, const Server *server = NULL); //error
	Response(const std::string& redir); //301
	Response(const std::string& cgiBody, bool hasBody); //cgi/post/delete
	Response(const Response& ref);
	Response& operator=(const Response& ref);
	~Response();

	const std::string					&getResponse() const;
	const std::string					&getResponseBody() const;
	const std::string					getRightContentType(const std::string suffix) const;

	void								setResponseBodyFromFile(const std::string& filename);
	void								setResponseBodyFromDir(const std::string& dirname);
	void								setResponseBodyFromError(int code, const std::string& errorStatus, const std::map<int, std::string>& errorPages);
	bool								isFinished();
	private: /* -Exception- */
		class ResponseException : public std::exception
		{
			private:
				int _code;
			public:
			ResponseException(int code) : _code(code) {}
			int	getError(void) const throw()
			{
				return (this->_code);
			}
		};
};

std::ostream&	operator<< (std::ostream& out, const Response& obj);

#endif
