#include "Cgi.hpp"
#include <cstdlib>
#include <unistd.h>

/*--------------------------------Coplien form--------------------------------*/
Cgi::Cgi()
{
	/*Constructor*/
}

Cgi::~Cgi()
{
	/*Destructor*/
}

Cgi::Cgi(const Cgi &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Cgi&	Cgi::operator=(const Cgi &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

static char *const	*prepare_env()
{
	std::vector<std::string> env;

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");	 // TODO: what value
	env.push_back("REMOTE_ADDR=127.0.0.1");		 // TODO: IP of the client
	env.push_back("REQUEST_METHOD=GET");		 // TODO: allow POST
	env.push_back("SCRIPT_NAME=" + path);		 //
	env.push_back("SERVER_NAME=127.0.0.1");		 // TODO: read from config
	env.push_back("SERVER_PORT=8080");			 // TODO: read from request
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");	 //
	env.push_back("SERVER_SOFTWARE=webserv/42"); //
	env.push_back("PATH_INFO=" + path_info);
	env.push_back("QUERY_STRING=" + query);

	return (static_cast<char *const *>(&env[0]);
}

Cgi::Cgi(std::string path, std::string query) : _cgiIn(-1), _cgiOut(-1), _pid(-1)
{
	int	pipeFD[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)

	// use pipes() to set input and output fd
	if (pipe(pipeFD) < 0)
		// throw error
	
	// use setenv to prepare the arguments for the cgi
	if (( _pid = fork()) == 0)
	{
		// child proccess
		// use execv to execute the cgi script
		if (execv(path.c_str(), prepare_env()) < 1);
			// throw error
		exit(1);
	}
	_cgiIn = pipeFD[0];
	_cgiOut = pipeFD[1];
}

