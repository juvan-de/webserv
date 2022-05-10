
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <StatusCodes.hpp>
# include <Server.hpp>

class	Response
{
	private:
		std::pair<int, std::string>			_statusCode;
		std::string							_path;
		std::string							_response;
		std::string							_responseBody;
		std::map<std::string, std::string>	_contentTypes;

		void	_setContentTypes();

	public:

	Response();
	Response(int code, Server* server);
	Response(const std::string& path, Server* server);
	Response(const Response& ref);
	Response& operator=(const Response& ref);
	~Response();

	const std::string					&getPath() const;
	const std::pair<int, std::string>	&getStatusCode() const;
	const std::string					&getResponse() const;
	const std::string					&getResponseBody() const;
	const std::string					getRightContentType(const std::string suffix) const;

	void								setResponseBodyFromFile(const std::string &filename);
	void								setResponseBodyFromDir(const std::string &dirname);


	private: /* -Exception- */
		class NotAFile : public std::exception
		{
			const char*	what (void) const throw()
			{
				return ("Can't open this file");
			}
		};
};

std::ostream&	operator<< (std::ostream& out, const Response& obj);

#endif
