#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <map>

class Cgi
{
	private:
		/*--------------------------Member variables--------------------------*/
		int		_cgiIn;
		int		_cgiOut;
		pid_t	_pid;
	public:
		/*----------------------------Coplien form----------------------------*/
		Cgi();
		Cgi(const Cgi &ref);
		Cgi& operator=(const Cgi &ref);
		~Cgi();

		/*--------------------------Member functions--------------------------*/
		Cgi(std::string path, std::string query);
};

#endif
