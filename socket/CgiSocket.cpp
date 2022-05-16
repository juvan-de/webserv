#include "CgiSocket.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <utils.hpp>

/*--------------------------------Coplien form--------------------------------*/

// CgiSocket::~CgiSocket()
// {
// 	/*Destructor*/
// }

// CgiSocket::CgiSocket(const CgiSocket &ref)
// {
// 	/*Copy constructor*/
// 	*this = ref;
// }

// CgiSocket&	CgiSocket::operator=(const CgiSocket &ref)
// {
// 	/*Assignation operator*/
// 	if (this != &ref)
// 	{
// 		/* assign member variables*/
// 	}
// 	return *this;
// }
/*--------------------------------Coplien form--------------------------------*/

static void			push_string(std::vector<char const*> &env, std::string str)
{
	env.push_back(str.c_str());
}

static std::string	getType(Type type)
{
	switch (type)
	{
		case GET:
			return "GET";
		case POST:
			return "POST";
		case DELETE:
			return "DELETE";
		default:
			return "";
	}
	return "";
}

CgiSocket::CgiSocket(Request request, Server server, sockaddr_in client_struct)
{
	/*Constructor*/
	std::stringstream			ss;
	std::string					cli_ip;
	std::string					req_type = getType(request.getType());
	std::string					filename = request.getLocation().substr(0, request.getLocation().find_first_of("?"));
	std::string					path;
	std::string					filepath;
	// std::vector<char const*>	env;

	ss << ntohs(client_struct.sin_family);
	ss >> cli_ip;
	char buf[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &client_struct.sin_family, buf, sizeof(buf)) == NULL)
		std::cout << "Error making ip" << std::endl;

	if (request.getLocation().find(".py?") != std::string::npos)
		path = server.getLocation("/").getCgi().find(".py")->second;
	else
		path = server.getLocation("/").getCgi().find(".php")->second;
	filepath = path + filename;

	push_string(_env, "GATEWAY_INTERFACE=CGI/1.1");
	push_string(_env, "REMOTE_ADDR=" + std::string(buf));
	push_string(_env, "REQUEST_METHOD=" + req_type);
	push_string(_env, "SCRIPT_NAME=" + filename);
	push_string(_env, "SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	push_string(_env, "SERVER_PORT=" + request.getHeaders().find("Host")->second);
	push_string(_env, "SERVER_PROTOCOL=HTTP/1.1");
	push_string(_env, "SERVER_SOFTWARE=webserv/42");
	// push_string(env, "PATH_INFO=" + path);
	// std::cout << "QUERY_STRING=" << request.getLocation().substr(request.getLocation().find_first_of("?") + 1, request.getLocation().size() - request.getLocation().find_first_of("?") - 1) << std::endl;
	push_string(_env, "QUERY_STRING=" + request.getLocation().substr(request.getLocation().find_first_of("?") + 1, request.getLocation().size() - request.getLocation().find_first_of("?") - 1));

	// std::cout << *this;

	executeCgi(filepath);
}

void		CgiSocket::executeCgi(std::string filepath)
{
	int	pipe_in[2]; // this will pipe from the cgi (pipefd[0] -> fd cgi script will write to file, pipefd[1] -> fd that serv will use to read the output from the written file)
	int	pipe_out[2];

	// use pipes() to set input and output fd
	if (pipe(pipe_in))
		std::cout << "Error: pipe didnt work" << std::endl;
		// throw error
	if (pipe(pipe_out))
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		std::cout << "Error: pipe didnt work" << std::endl;
	}
	
	// use setenv to prepare the arguments for the cgi
	std::cout << "CGI STARTS HERE" << std::endl;
	pid_t pid = fork();
	std::cout << "what: " << pid << std::endl;
	if (pid == 0)
	{
		// child proccess
		dup2(pipe_in[0], STDIN_FILENO); // set correct input to pipe
		dup2(pipe_out[1], STDOUT_FILENO); // set correct output from pipe
		dup2(pipe_out[1], STDERR_FILENO);

		// use execv to execute the cgi script
		// std::cout << "filepath: " << filepath << std::endl;
		if (execve(filepath.c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)_env.data()) < 0)
			std::cout << "Error: couldnt execv: " << errno << ", path: " << filepath << std::endl;
			// throw error
		
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		exit(0);
	}
	else
	{
		//create newcli met fd en zelfde request
		char str[9999];
		int end;
		end = read(pipe_out[0], str, 9999);
		std::cout << "READ RETURN: " << end << std::endl;
		str[end] = '\0';
		std::cout << "CGI" << std::endl << std::string(str) << std::endl << "CGI" << std::endl;
		close(pipe_in[0]);
		// close(pipe_in[1]);
		// close(pipe_out[0]);
		close(pipe_out[1]);
	}
}