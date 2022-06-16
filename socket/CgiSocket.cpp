#include "CgiSocket.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <utils.hpp>
#include <sys/stat.h> // stat struct
#include <errno.h>
#include <Request.hpp>
#define BUFFER_SIZE 100
/*--------------------------------Coplien form--------------------------------*/

CgiSocket::~CgiSocket()
{
	/*Destructor*/
	// std::cout << "DEBUG: CGI SOCK CLOSED" << std::endl;
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

CgiSocket::CgiSocket(std::string filename, Request request, Location location, sockaddr_in client_struct)
	 : _status(CREATED), _hasBody(false), _input(std::string()), _output(std::string())
{
	// std::cout << "DEBUG: CGI SOCK OPENED" << std::endl;
	/*Constructor*/
	std::stringstream			ss;
	std::string					body_len;
	std::string					req_type = getType(request.getType());
	char						buf[INET_ADDRSTRLEN];

	if (inet_ntop(AF_INET, &client_struct.sin_family, buf, sizeof(buf)) == NULL)
		throw Request::RequestException(500);
	if (request.getBody().size() > 0)
	{
		_hasBody = true;
		_input = request.getBody();
	}
	_filepath = getFilepath(filename, location);
	ss << request.getBody().size();
	ss >> body_len;

	_envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_envp.push_back("REMOTE_ADDR=" + std::string(buf));
	_envp.push_back("REQUEST_METHOD=" + req_type);
	_envp.push_back("CONTENT_LENGTH=" + body_len);
	_envp.push_back("CONTENT_TYPE=text/html"); // need to get actual type and only include when there is a body
	_envp.push_back("SCRIPT_NAME=" + request.getUri().substr(0, request.getUri().find_first_of("?")));
	_envp.push_back("SERVER_NAME=" + request.getHeaders().find("Referer")->second);
	_envp.push_back("SERVER_PORT=" + request.getHeaders().find("Host")->second);
	_envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_envp.push_back("SERVER_SOFTWARE=webserv/42");
	_envp.push_back("QUERY_STRING=" + request.getUri().substr(request.getUri().find_first_of("?") + 1, request.getUri().size() - request.getUri().find_first_of("?") - 1));

	prepareCgi();
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

static bool	doesDirExist(const std::string& dirname)
{
	struct stat	stats;
	int			ret;

	ret = stat(dirname.c_str(), &stats);
	if (ret == 0 && S_ISDIR(stats.st_mode))
		return true;
	return false;
}

std::string	CgiSocket::getFilepath(std::string filename, Location location)
{
	std::map<std::string, std::string>::const_iterator	path;
	std::string											root;
	std::string											filepath;
	int 												ret;

	path = location.getCgi().find(filename.substr(filename.find("."), filename.size()));
	if (path == location.getCgi().end())
		throw Request::RequestException(400);
	root = location.getRoot();
	if (!doesDirExist(root + "/" + path->second))
		throw Request::RequestException(404);
	filepath = root + "/" + path->second + filename;
	if ((ret = isValidPath(root, filepath)) != 0)
		throw Request::RequestException(ret);
	return filepath;
}

void	CgiSocket::childProccess()
{
	dup2(_pipeIn[0], STDIN_FILENO);
	dup2(_pipeOut[1], STDOUT_FILENO);
	dup2(_pipeOut[1], STDERR_FILENO);

	if (execve(_filepath.c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)vec_to_arr(_envp).data()) < 0)
		std::cout << "Execv error: " << errno << std::endl; // forbidden 403(errno = 2), 500 Internal Server Error (1, 3, 4, 5, error), bad request 400 (13)
	
	close(_pipeIn[0]);
	close(_pipeIn[1]);
	close(_pipeOut[0]);
	close(_pipeOut[1]);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	CgiSocket::mainProcess()
{
	_fdOut = _pipeOut[0];
	_fdIn = _pipeIn[1];
}

void	CgiSocket::executeCgi()
{
	try
	{
		pid_t pid = fork();
		if (pid == 0)
			childProccess();
		else
			mainProcess();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

/*-------------------------------statics/private------------------------------*/

void	CgiSocket::prepareCgi()
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
	_pipeIn[0] = pipe_in[0];
	_pipeIn[1] = pipe_in[1];
	_pipeOut[0] = pipe_out[0];
	_pipeOut[1] = pipe_out[1];
	_fdOut = _pipeOut[0];
	_fdIn = _pipeIn[1];

	int flags;
	if ((flags = fcntl(_fdOut, F_GETFL)) < 0)
		throw CgiException(500);
	if (fcntl(_fdOut, F_SETFL, O_NONBLOCK) < 0)
		throw CgiException(500);
	if ((flags = fcntl(_fdIn, F_GETFL)) < 0)
		throw CgiException(500);
	if (fcntl(_fdIn, F_SETFL, O_NONBLOCK) < 0)
		throw CgiException(500);

	if (!_hasBody)
		executeCgi();
}

void	CgiSocket::read_from_cgi()
{
	char	cstr[BUFFER_SIZE + 1];
	int		ret = 1;

	ret = read(getFdOut(), cstr, BUFFER_SIZE);
	if (ret < 0)
		throw CgiException(500);
	if (ret < BUFFER_SIZE && ret >= 0) {
		_status = FINISHED;
		// exit(1);
	}
	cstr[ret] = '\0';
	_output.append(cstr);
	// std::cout << "*********input*********\n" << this->_output << "\n*********input*********" << "\nret: " << ret << std::endl;
}

void	CgiSocket::write_to_cgi()
{
	int		ret = 1;

	ret = write(getFdIn(), _input.c_str(), _input.size());
	if (ret < 0)
		throw CgiException(500);
	executeCgi();
	_hasBody = false;
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
	if (_output.size() == 0)
		throw CgiException(500);
}