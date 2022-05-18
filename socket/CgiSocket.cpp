#include "CgiSocket.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <utils.hpp>

#define BUFFER_SIZE 30
/*--------------------------------Coplien form--------------------------------*/

CgiSocket::~CgiSocket()
{
	/*Destructor*/
	std::cout << "WTF******************************************" << std::endl;
	close(_fdOut[0]);
	close(_fdOut[1]);
}

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
static std::vector<const char*> vec_to_arr(const std::vector<std::string>& tmp)
{
	std::vector<const char*> envp;
	envp.reserve(tmp.size() + 1);
	for (size_t i = 0; i < tmp.size(); i++)
		envp.push_back(tmp[i].c_str());
	envp.push_back(NULL);
	return envp;
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

CgiSocket::CgiSocket(Request request, Server server, sockaddr_in client_struct) : _status(CREATED), _input(std::string())
{
	/*Constructor*/
	std::string					req_type = getType(request.getType());
	std::string					filename = request.getLocation().substr(0, request.getLocation().find_first_of("?"));
	std::string					path;
	std::string					filepath;
	std::vector<std::string>	tmp;
	std::vector<const char*>	envp;

	char buf[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &client_struct.sin_family, buf, sizeof(buf)) == NULL)
		std::cout << "Error making ip" << std::endl;

	if (request.getLocation().find(".py?") != std::string::npos)
		path = server.getLocation("/").getCgi().find(".py")->second;
	else
		path = server.getLocation("/").getCgi().find(".php")->second;
	filepath = path + filename;

	tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	tmp.push_back("REMOTE_ADDR=" + std::string(buf));
	tmp.push_back("REQUEST_METHOD=" + req_type);
	tmp.push_back("SCRIPT_NAME=" + filename);
	tmp.push_back("SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	tmp.push_back("SERVER_PORT=" + request.getHeaders().find("Host")->second);
	tmp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	tmp.push_back("SERVER_SOFTWARE=webserv/42");
	tmp.push_back("QUERY_STRING=" + request.getLocation().substr(request.getLocation().find_first_of("?") + 1, request.getLocation().size() - request.getLocation().find_first_of("?") - 1));

	executeCgi(filepath, tmp);
}

void		CgiSocket::executeCgi(std::string filepath, std::vector<std::string> envp)
{
	int	pipe_in[2];
	int	pipe_out[2];

	if (pipe(pipe_in))
		std::cout << "Error: pipe didnt work" << std::endl;
	if (pipe(pipe_out))
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		std::cout << "Error: pipe didnt work" << std::endl;
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);
		dup2(pipe_out[1], STDERR_FILENO);

		if (execve(filepath.c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)vec_to_arr(envp).data()) < 0)
			std::cout << "Error: couldnt execv: " << errno << ", path: " << filepath << std::endl;
		
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
		// char str[9999];
		// int end;
		// end = read(pipe_out[0], str, 9999);
		// std::cout << "READ RETURN: " << end << std::endl;
		// str[end] = '\0';
		// std::cout << "CGI" << std::endl << std::string(str) << std::endl << "CGI" << std::endl;
		close(pipe_in[0]);
		close(pipe_in[1]);
		// close(pipe_out[1]);
		// _fdIn = pipe_in[1];
		_fdOut[0] = pipe_out[0];
		_fdOut[1] = pipe_out[1];
		// int flags;
		// if ((flags = fcntl(_fdOut, F_GETFL)) < 0)
		// 	std::cout << "couldnt get flags" << std::endl;
		// if (fcntl(_fdOut, F_SETFL, flags | O_NONBLOCK) < 0)
		// 	std::cout << "couldnt set flags" << std::endl;
	}
}

void		CgiSocket::read_cgi()
{
	char	cstr[BUFFER_SIZE + 1];
	int		ret = 1;

	// this can return an error if operation would block, see man page
	ret = read(getFd(), cstr, BUFFER_SIZE);
	if (ret > 0)
	{
		cstr[ret] = '\0';
		_input.append(cstr);
		std::cout << "*********input*********\n" << this->_input << "\n*********input*********" << "\nret: " << ret << std::endl;
	}
	else if (ret <= -1)
		std::cout << "\033[31m" << "READ ERROR: " << ret << "\033[0m" << std::endl;
}
