#include "CgiSocket.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <utils.hpp>
#include <errno.h>
#define BUFFER_SIZE 30
/*--------------------------------Coplien form--------------------------------*/

CgiSocket::~CgiSocket()
{
	/*Destructor*/
	std::cout << "DEBUG: CGI SOCK CLOSED" << std::endl;
	close(_fdOut[0]);
	close(_fdOut[1]);
}

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
	std::string					root = server.getLocation("/").getRoot();
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
	// filepath = root + path + filename;
	filepath = path + filename; // still need to add realpath to be able to do root + path + filename
	std::cout << "CGI filepath: " << filepath << std::endl;

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
		std::cout << "CGI ERROR 500" << std::endl;
	if (pipe(pipe_out))
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		std::cout << "CGI ERROR 500" << std::endl;
	}

	try
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			dup2(pipe_in[0], STDIN_FILENO);
			dup2(pipe_out[1], STDOUT_FILENO);
			dup2(pipe_out[1], STDERR_FILENO);

			if (execve(filepath.c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)vec_to_arr(envp).data()) < 0)
				std::cout << "Execv error: " << errno << std::endl; // forbidden 403(errno = 2), 500 Internal Server Error (1, 3, 4, 5, error), bad request 400 (13)
			
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
			close(pipe_in[0]);
			close(pipe_in[1]);
			_fdOut[0] = pipe_out[0];
			_fdOut[1] = pipe_out[1];
			int flags;
			if ((flags = fcntl(getFd(), F_GETFL)) < 0)
				std::cout << "wtf" << std::endl;
			if (fcntl(getFd(), F_SETFL, flags | O_NONBLOCK) < 0)
				std::cout << "wtf" << std::endl;
		}	
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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
	else if (ret < BUFFER_SIZE && ret >= 0)
		_status = FINNISHED;
	else if (ret <= -1)
		std::cout << "\033[31m" << "READ ERROR: " << ret << "\033[0m" << std::endl;
}

void	CgiSocket::checkError()
{
	std::stringstream ss;
	int error;
	std::string num = "";

	if (_input.find("Execv error: ") != std::string::npos)
	{
		// forbidden 403(errno = 2), 500 Internal Server Error (1, 3, 4, 5, error), bad request 400 (13)
		for (size_t i = 13; i < _input.size() && isdigit(_input[i]); i++)
			num.push_back(_input[i]);
		ss << num;
		ss >> error;

		switch (error)
		{
		case 2:
			std::cout << "CGI ERROR 403" << std::endl;
			break;
		case 13:
			std::cout << "CGI ERROR 400" << std::endl;
			break;
		default:
			std::cout << "CGI ERROR 500" << std::endl;
			break;
		}
	}
}