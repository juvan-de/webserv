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

static char *const	*prepare_env(Request request, std::string client_ip)
{
	std::vector<char *const> env;

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");	 // TODO: what value
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");	 // TODO: what value
	env.push_back("REMOTE_ADDR=" + client_ip);		 // TODO: IP of the client
	env.push_back("REQUEST_METHOD=" + request.getType());		 // TODO: allow POST
	env.push_back("SCRIPT_NAME=" + request.getCgi());
	env.push_back("SERVER_NAME=" + request.getHeaders("Referer"));		 // TODO: read from config
	env.push_back("SERVER_PORT=" + request.getHeaders("Host"));			 // TODO: read from request
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_SOFTWARE=webserv/42");
	env.push_back("PATH_INFO=" + request.getPath());
	env.push_back("QUERY_STRING=" + request.getQuery());

	return static_cast<char *const *>(&env[0]);
}

Cgi::Cgi(Request request, std::string client_ip)
{
	int	pipeFD[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)
	int pid, cgiFd;

	// use pipes() to set input and output fd
	if (pipe(pipeFD) < 0)
		// throw error
	
	// use setenv to prepare the arguments for the cgi
	if (( pid = fork()) == 0)
	{
		// child proccess
		dup2(pipeFD[0], STDIN_FILENO); // set correct input to pipe
		dup2(cgiFd, STDOUT_FILENO); // set correct output from pipe
		dup2(cgiFd, STDERR_FILENO);

		// use execv to execute the cgi script
		if (execv(path.c_str(), prepare_env(request, client_ip)) < 1);
			// throw error
		
		close(pipeFD[0]);
		close(pipeFD[1]);
		close(cgiFd);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		exit(0);
	}
	else
	{
		//create newcli met fd en zelfde request
		close(pipeFD[0]);
		close(pipeFD[1]);

	}
}

