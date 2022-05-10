#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <map>
# include <Request.hpp>
# include <Server.hpp>
# include <netinet/in.h>

class Request;

class Cgi
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::vector<char *const>	_env;
		int							_cgiFd;
		pid_t						_pid;

		void	add_string(std::string str);
	public:
		/*----------------------------Coplien form----------------------------*/
		Cgi();
		Cgi(const Cgi &ref);
		Cgi& operator=(const Cgi &ref);
		~Cgi();

		/*--------------------------Member functions--------------------------*/
		Cgi(Request request, Server server, sockaddr client_struct);
		void	executeCgi();
		const std::vector<char *const>	getEnv() const { return _env; }
};

std::ostream&	operator<<(std::ostream &out, const Cgi &ref);


#endif
