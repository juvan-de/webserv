#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <map>
# include <Request.hpp>

class Request;

class Cgi
{
	private:
		/*--------------------------Member variables--------------------------*/
		int		_cgiFd;
		int		_clientfd;
		pid_t	_pid;
	public:
		/*----------------------------Coplien form----------------------------*/
		Cgi();
		Cgi(const Cgi &ref);
		Cgi& operator=(const Cgi &ref);
		~Cgi();

		/*--------------------------Member functions--------------------------*/
		Cgi(Request request, std::string client_ip);
};

#endif
