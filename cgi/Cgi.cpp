#include "Cgi.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
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
	std::cout << "copy constreqgr	qej" << std::endl;
	*this = ref;
}

Cgi&	Cgi::operator=(const Cgi &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		for (std::vector<std::string>::const_iterator it = ref.getEnv().begin(); it != ref.getEnv().end(); it++) {
			std::cout << "DEBUG " << *it << std::endl;
			this->_env.push_back(*it);
		}
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

void	Cgi::executeCgi()
{
	// int	pipeFD[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)
	// int pid, cgiFd;
	// std::string file = strtrim(request.getLocation(), "?");

	// // use pipes() to set input and output fd
	// if (pipe(pipeFD) < 0)
	// 	std::cout << "Error: pipe didnt work" << std::endl;
	// 	// throw error
	
	// // use setenv to prepare the arguments for the cgi
	// if (( pid = fork()) == 0)
	// {
	// 	// child proccess
	// 	dup2(pipeFD[0], STDIN_FILENO); // set correct input to pipe
	// 	dup2(cgiFd, STDOUT_FILENO); // set correct output from pipe
	// 	dup2(cgiFd, STDERR_FILENO);

	// 	// use execv to execute the cgi script
	// 	if (execv((path + file).c_str(), prepare_env(path, file, request, client_ip, *request.getLocation)) < 1);
	// 		std::cout << "Error: couldnt execv" << std::endl;
	// 		// throw error
		
	// 	close(pipeFD[0]);
	// 	close(pipeFD[1]);
	// 	close(cgiFd);
	// 	close(STDOUT_FILENO);
	// 	close(STDERR_FILENO);
	// 	exit(0);
	// }
	// else
	// {
	// 	//create newcli met fd en zelfde request
	// 	close(pipeFD[0]);
	// 	close(pipeFD[1]);

	// }
}

// char *const			*Cgi::prepare_env()
// {
// 	char *const *env;
// 	// env.reserve(_env.size());

// 	for (std::vector<std::string>::const_iterator it = _env.begin(); it != _env.end(); it++)
// 		env.push_back(const_cast<char *const>((*it).c_str()));
	
// 	return static_cast<char *const *>(&env[0]);
// }

Cgi::Cgi(Request request, Server server, sockaddr client_struct)
{
	std::stringstream			ss;
	std::string					cli_ip;
	std::string					req_type;

	ss << ntohs(client_struct.sa_family);
	ss >> cli_ip;

	switch (request.getType())
	{
		case GET:
			req_type = "GET";
			break;
		case POST:
			req_type = "POST";
			break;
		case DELETE:
			req_type = "DELETE";
			break;
		default:
			break;
	}

	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("REMOTE_ADDR=" + cli_ip);
	_env.push_back("REQUEST_METHOD=" + req_type); 
	_env.push_back("SCRIPT_NAME=" + request.getLocation().substr(0, request.getLocation().find_first_of("?")));
	_env.push_back("SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	_env.push_back("SERVER_PORT=" + request.getHeaders().find("Host")->second);
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_SOFTWARE=webserv/42");
	if (request.getLocation().find(".py?") != std::string::npos)
		_env.push_back("PATH_INFO=" + server.getLocation("/").getCgi().find(".py")->second);
	else
		_env.push_back("PATH_INFO=" + server.getLocation("/").getCgi().find(".php")->second);
	_env.push_back("QUERY_STRING=" + request.getLocation().substr(request.getLocation().find_first_of("?") + 1, request.getLocation().size() - request.getLocation().find_first_of("?") - 1));

	executeCgi();
}

std::ostream&	operator<<(std::ostream &out, const Cgi &ref)
{
	std::vector<std::string> tmp = ref.getEnv();
	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		out << *it << std::endl;

	return out;
}