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
	close(_pipeIn[0]);
	close(_pipeIn[1]);
	close(_pipeOut[0]);
	close(_pipeOut[1]);
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

CgiSocket::CgiSocket(Request request, Server server, sockaddr_in client_struct) : _status(CREATED), _output(std::string())
{
	/*Constructor*/
	std::string					filepath;
	std::vector<std::string>	envp;
	std::string					req_type = getType(request.getType());
	char						buf[INET_ADDRSTRLEN];

	if (inet_ntop(AF_INET, &client_struct.sin_family, buf, sizeof(buf)) == NULL)
		std::cout << "Error making ip" << std::endl;
	if (request.getType() == POST && request.getBody().size())
		std::cout << "entered post request" << std::endl;
	filepath = getFilepath(server, request);

	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envp.push_back("REMOTE_ADDR=" + std::string(buf));
	envp.push_back("REQUEST_METHOD=" + req_type);
	envp.push_back("SCRIPT_NAME=" + request.getUri().substr(0, request.getUri().find_first_of("?")));
	envp.push_back("SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	envp.push_back("SERVER_PORT=" + request.getHeaders().find("Host")->second);
	envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envp.push_back("SERVER_SOFTWARE=webserv/42");
	envp.push_back("QUERY_STRING=" + request.getUri().substr(request.getUri().find_first_of("?") + 1, request.getUri().size() - request.getUri().find_first_of("?") - 1));

	executeCgi(filepath, envp);
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

std::string	CgiSocket::getFilepath(Server server, Request request)
{
	std::map<std::string, Location>::const_iterator 	location;
	std::map<std::string, std::string>::const_iterator	path;
	std::string											root;
	std::string											filename;
	std::string											filepath;
	int 												ret;
	
	location = server.getRightLocation("/");
	if (location == server.getLocations().end())
		throw CgiException(404);
	if (request.getUri().find(".py?") != std::string::npos)
		path = location->second.getCgi().find(".py");
	else
		path = location->second.getCgi().find(".php");
	if (path == location->second.getCgi().end())
		throw CgiException(400);
	
	root = location->second.getRoot();
	filename = request.getUri().substr(0, request.getUri().find_first_of("?"));
	filepath = root + "/" + path->second + filename;

	if ((ret = isValidPath(root, filepath)) != 0)
		throw CgiException(ret);
	return filepath;
}

static void	childProccess(int pipe_in[2], int pipe_out[2], std::string filepath, std::vector<std::string> envp)
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
}

void	CgiSocket::mainProcess(int pipe_in[2], int pipe_out[2])
{
	_pipeIn[0] = pipe_in[0];
	_pipeIn[1] = pipe_in[1];
	_pipeOut[0] = pipe_out[0];
	_pipeOut[1] = pipe_out[1];
	_fdOut = pipe_out[0];
	_fdIn = pipe_in[1];
	if (fcntl(_fdOut, F_SETFL, O_NONBLOCK) < 0)
		throw CgiException(500);
	if (fcntl(_fdIn, F_SETFL, O_NONBLOCK) < 0)
		throw CgiException(500);
}
/*-------------------------------statics/private------------------------------*/

void	CgiSocket::executeCgi(std::string filepath, std::vector<std::string> envp)
{
	int	pipe_in[2];
	int	pipe_out[2];

	if (pipe(pipe_in))
		throw CgiException(500);
	if (pipe(pipe_out))
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		throw CgiException(500);
	}

	try
	{
		pid_t pid = fork();
		if (pid == 0)
			childProccess(pipe_in, pipe_out, filepath, envp);
		else
			mainProcess(pipe_in, pipe_out);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

void	CgiSocket::read_from_cgi()
{
	char	cstr[BUFFER_SIZE + 1];
	int		ret = 1;

	ret = read(getFdOut(), cstr, BUFFER_SIZE);
	if (ret > 0)
	{
		cstr[ret] = '\0';
		_output.append(cstr);
		std::cout << "*********input*********\n" << this->_output << "\n*********input*********" << "\nret: " << ret << std::endl;
	}
	else if (ret < BUFFER_SIZE && ret >= 0)
		_status = FINISHED;
	else if (ret <= -1)
		throw CgiException(500);
}

void	CgiSocket::write_to_cgi()
{
	std::string input;
	int		ret = 1;

	ret = write(getFdIn(), input.c_str(), input.size());
	if (ret <= -1)
		throw CgiException(500);
}

void	CgiSocket::checkError()
{
	std::stringstream ss;
	int error;
	std::string num = "";

	if (_output.find("Execv error: ") != std::string::npos)
	{
		// forbidden 403(errno = 2), 500 Internal Server Error (1, 3, 4, 5, error), bad request 400 (13)
		for (size_t i = 13; i < _output.size() && isdigit(_output[i]); i++)
			num.push_back(_output[i]);
		ss << num;
		ss >> error;

		switch (error)
		{
		case 2:
			throw CgiException(403);
		case 13:
			throw CgiException(400);
		default:
			throw CgiException(500);
		}
	}
}