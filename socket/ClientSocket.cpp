#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <sys/stat.h> // stat struct

ClientSocket::ClientSocket(int fd, sockaddr addr) :
	 Socket(fd), _status(200), _request(Request())
{
	int flags;
	int opt = 1;

	// Prepare socket
	if ((flags = fcntl(getFd(), F_GETFL)) < 0)
		throw BadInit();
	if (fcntl(getFd(), F_SETFL, flags | O_NONBLOCK) < 0)
		throw BadInit();
	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw BadInit();

	// Set address struct
	bzero(&_address, sizeof(_address));
	_address = addr;
}

void	ClientSocket::handle_pollin()
{
	std::cout << "POLLING IN" << std::endl;
	_request.addto_request(getFd());
	if (_request.getType() == NOTSET)
	{
		_request.setRequest();
		_request.setHeaders();
	}
	if (_request.checkIfChunked())
	{
		// std::cout << "CHUNKED" << std::endl;
		_request.readChunked(getFd());
	}
}

/*----------------------------------------POLLOUT--------------------------------------------*/

Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request Request)
{
	std::map<std::string, std::string> headers = Request.getHeaders();
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding hostname" << std::endl;
		return NULL;
	}
	std::string host = headers["Host"];
	std::string name = host.substr(0, host.find(":"));
	int port = std::atoi(host.substr(host.find(":") + 1).c_str());
	if (table.find(std::make_pair(port, name)) == table.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding pair" << std::endl;
		return NULL;
	}
	return (table[std::make_pair(port, name)]);
}

std::string	getFileName(const Location& loc)
{
	struct stat	buf;
	int			ret;
	std::string res;
	std::string filename;
	
	// std::cout << "Root: " << loc.getRoot() << std::endl;
	// std::cout << "Title: " << loc.getTitle() << std::endl;

	res = loc.getRoot() + loc.getTitle();
	for (std::vector<std::string>::const_iterator itr = loc.getIndex().begin(); itr != loc.getIndex().end(); itr++)
	{
		if (loc.getTitle().size() == 1 && loc.getTitle()[0] == '/')
			filename = res + *itr;
		else
			filename = res + "/" + *itr;
		ret = stat(filename.c_str(), &buf);
		std::cout << filename << std::endl;
		if (ret == 0)
			return filename;
	}
	/* bad request, wa gaan we hier doen */
	std::cout << "bad request (getFileName)" << std::endl;
	return NULL;
}

void	remove_last_dir(std::string& request_loc)
{
	request_loc = request_loc.substr(0, request_loc.find_last_of("/"));
}

std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>& locations, std::string request_loc)
{
	while (true)
	{
		if (locations.find(request_loc) != locations.end())
			return locations.find(request_loc);
		remove_last_dir(request_loc);
		if (request_loc.empty())
			return (locations.find("/"));
	}
}

void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table, Poller &poll)
{
	std::cout << "POLLING OUT" << std::endl;

	Server *server = find_server(table, _request);
	if (_request.getType() == GET)
	{
		/* for now */
		std::map<std::string, Location>::const_iterator itr = find_right_location(server->getLocations(), _request.getLocation());
		if (itr == server->getLocations().end())
		{
			/* bad request */
			std::cout << "DEBUG HANDLE RESPONSE: location: [" << _request.getLocation() << "]" <<std::endl;
			std::cout << "bad request" << std::endl;
			return ;
		}
		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
		{
			/* bad request (405 forbidden)*/
			std::cout << "bad request (forbidden)" << std::endl;
			return ;
		}
		std::string filename = getFileName(itr->second);
		Response response = Response(filename, server);
//		std::cout << response << std::endl;
		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
//		std::cout << ret << "\t" << response.getResponse().length() << std::endl;
		_request = Request();
	}
	else if (_request.getType() == POST)
	{
		std::cout << "Post request" << std::endl;
		std::cout << _request.getLocation() << std::endl;
	}
	else if (_request.getType() == DELETE)
	{
		std::cout << "we be deletin tho" << std::endl;
	}
	else
	{
		std::cout << "shit went wrong yo" << std::endl;
	}
}
