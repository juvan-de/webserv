#include "Cgi.hpp"
#include <cstdlib>
#include <unistd.h>
#include <utils.hpp>

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

static void			add_string(std::vector<char *const> &env, std::string str)
{
	env.push_back(const_cast<char *const>(str.c_str()));
}

static char *const	*prepare_env(std::string path, std::string file, Request request, std::string client_ip, std::string query)
{
	std::vector<char *const> env;

	add_string(env, "GATEWAY_INTERFACE=CGI/1.1");
	add_string(env, "REMOTE_ADDR=" + client_ip); // client ip
	add_string(env, "REQUEST_METHOD=" + request.getType()); 
	add_string(env, "SCRIPT_NAME=" + file);
	add_string(env, "SERVER_NAME=" + request.getHeaders("Referer"));
	add_string(env, "SERVER_PORT=" + request.getHeaders("Host"));
	add_string(env, "SERVER_PROTOCOL=HTTP/1.1");
	add_string(env, "SERVER_SOFTWARE=webserv/42");
	add_string(env, "PATH_INFO=" + path);
	add_string(env, "QUERY_STRING=" + query);

	return static_cast<char *const *>(&env[0]);
}

Cgi::Cgi(std::string path, Request request, std::string client_ip)
{
	int	pipeFD[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)
	int pid, cgiFd;
	std::string file = strtrim(request.getLocation(), "?");

	// use pipes() to set input and output fd
	if (pipe(pipeFD) < 0)
		std::cout << "Error: pipe didnt work" << std::endl;
		// throw error
	
	// use setenv to prepare the arguments for the cgi
	if (( pid = fork()) == 0)
	{
		// child proccess
		dup2(pipeFD[0], STDIN_FILENO); // set correct input to pipe
		dup2(cgiFd, STDOUT_FILENO); // set correct output from pipe
		dup2(cgiFd, STDERR_FILENO);

		// use execv to execute the cgi script
		if (execv((path + file).c_str(), prepare_env(path, file, request, client_ip, *request.getLocation)) < 1);
			std::cout << "Error: couldnt execv" << std::endl;
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

