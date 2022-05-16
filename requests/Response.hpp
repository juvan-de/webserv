
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <StatusCodes.hpp>
# include <Server.hpp>

class	Response
{
	private:
		std::string							_response;
		std::string							_responseBody;
		std::map<std::string, std::string>	_contentTypes;

		void	_setContentTypes();
		void	_makeDefaultErrorPage(std::pair<int, std::string> errcode);

	public:

	Response();
	Response(const Server *server, const std::string path); //valid
	Response(int errorcode, const Server *server = NULL); //error
	Response(const std::string& redir); //301
	Response(const Response& ref);
	Response& operator=(const Response& ref);
	~Response();

	const std::string					&getResponse() const;
	const std::string					&getResponseBody() const;
	const std::string					getRightContentType(const std::string suffix) const;

	void								setResponseBodyFromFile(const std::string& filename);
	void								setResponseBodyFromDir(const std::string& dirname);
	void								setResponseBodyFromError(int code, const std::map<int, std::string>& errorPages);

	private: /* -Exception- */
		class ResponseException : public std::exception
		{
			private:
				int _code;
			public:
			ResponseException(int code) : _code(code) {}
			const int	getError(void) const throw()
			{
				return (this->_code);
			}
		};
};

std::ostream&	operator<< (std::ostream& out, const Response& obj);

#endif
