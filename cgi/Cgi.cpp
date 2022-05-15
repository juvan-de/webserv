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
		for (std::vector<char const*>::const_iterator it = ref.getEnv().begin(); it != ref.getEnv().end(); it++) {
			std::cout << "DEBUG " << *it << std::endl;
			this->push_string(*it);
		}
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

void	Cgi::executeCgi()
{
	int	pipeFD[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)
	int pid, cgiFd;

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
		if (execv(_filepath.c_str(), const_cast<char *const *>(&_env[0])) < 1)
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
		char str[9999];
		read(cgiFd, str, 9999);
		std::cout << "CGI" << std::endl << std::string(str) << std::endl << "CGI" << std::endl;
		close(pipeFD[0]);
		close(pipeFD[1]);
	}
}

void	Cgi::push_string(std::string str)
{
	_env.push_back(str.c_str());
}

Cgi::Cgi(Request request, Server server, sockaddr_in client_struct)
{
	std::stringstream			ss;
	std::string					cli_ip;
	std::string					req_type;
	std::string					filename;
	std::string					path;

	ss << ntohs(client_struct.sin_family);
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

	push_string("GATEWAY_INTERFACE=CGI/1.1");
	push_string("REMOTE_ADDR=" + cli_ip);
	push_string("REQUEST_METHOD=" + req_type);
	filename = request.getLocation().substr(0, request.getLocation().find_first_of("?"));
	push_string("SCRIPT_NAME=" + filename);
	push_string("SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	push_string("SERVER_PORT=" + request.getHeaders().find("Host")->second);
	push_string("SERVER_PROTOCOL=HTTP/1.1");
	push_string("SERVER_SOFTWARE=webserv/42");
	if (request.getLocation().find(".py?") != std::string::npos)
		path = server.getLocation("/").getCgi().find(".py")->second;
	else
		path = server.getLocation("/").getCgi().find(".php")->second;
	push_string("PATH_INFO=" + path);
	push_string("QUERY_STRING=" + request.getLocation().substr(request.getLocation().find_first_of("?") + 1, request.getLocation().size() - request.getLocation().find_first_of("?") - 1));

	_filepath = path + filename;

	std::cout << *this;

	executeCgi();
}

std::ostream&	operator<<(std::ostream &out, const Cgi &ref)
{
	std::vector<char const*> tmp = ref.getEnv();
	out << "--------------------CGI--------------------------" << std::endl;
	out << "FilPath = " << ref.getFilepath() << std::endl;
	for (std::vector<char const*>::iterator it = tmp.begin(); it != tmp.end(); it++) {
		std::cout << *it << std::endl;
	}
		// out << std::string(*it) << std::endl;
	out << "--------------------CGI--------------------------" << std::endl;
	return out;
}