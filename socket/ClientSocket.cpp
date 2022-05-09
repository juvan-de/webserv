#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <utils.hpp>

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
		std::cout << filename << std::endl;
		if (doesFileExist(filename))
			return filename;
	}
	/* bad request, wa gaan we hier doen */
	std::cout << "bad request (getFileName)" << std::endl;
	return NULL;
}

// void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
// {
// 	std::cout << "POLLING OUT" << std::endl;

// 	Server *server = find_server(table, _request); // we could put the server in the client object to avoid having to find the right server for every request.
// 	if (_request.getType() == GET)
// 	{
// 		/* for now */
// 		std::string	filename;
// 		std::string	request_location = this->_request.getLocation();
// 		server->getRightLocation(request_location);
// 		std::map<std::string, Location>::const_iterator itr = find_right_location(server->getLocations(), request_location);
// 		std::cout << "request_location: " << request_location << std::endl;
// 		if (itr == server->getLocations().end())
// 		{
// 			/* bad request */
// 			std::cout << "DEBUG HANDLE RESPONSE: location: [" << request_location << "]" <<std::endl;
// 			std::cout << "bad request" << std::endl;
// 			return ;
// 		}
// 		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
// 		{
// 			/* bad request (405 forbidden)*/
// 			std::cout << "bad request (forbidden)" << std::endl;
// 			return ;
// 		}
// 		filename = itr->second.getRoot() + request_location;
// 		if (!doesFileExits(filename))
// 		{
// 			std::cout << "file does not exits" << std::endl;
// 			exit(1);
// 		}
// 		std::cout << "FILENAME: " << filename << std::endl;
// 		Response response = Response(filename, server);
// //		std::cout << response << std::endl;
// 		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
// //		std::cout << ret << "\t" << response.getResponse().length() << std::endl;
// 		_request = Request();
// 	}
// 	else if (_request.getType() == POST)
// 	{

// 	}
// 	else if (_request.getType() == DELETE)
// 	{
// 		std::cout << "we be deletin tho" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "shit went wrong yo" << std::endl;
// 	}
// }



void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
{
	std::cout << "POLLING OUT" << std::endl;

	Server *server = find_server(table, _request); // we could put the server in the client object to avoid having to find the right server for every request.
	if (_request.getType() == GET)
	{
		std::string	filename;
		std::string	request_location = this->_request.getLocation();
		std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
		if (itr == server->getLocations().end())
		{
			/* bad request */
			std::cout << "DEBUG HANDLE RESPONSE: location: [" << request_location << "]" <<std::endl;
			std::cout << "bad request" << std::endl;
			return ;
		}
		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
		{
			/* bad request (405 forbidden)*/
			std::cout << "bad request (forbidden)" << std::endl;
			return ;
		}
		if (request_location[request_location.size() - 1] == '/')
		{
			//iterator return
			std::vector<std::string>::const_iterator itr_fileame = itr->second.getRightIndexFile(itr->second.getRoot() + request_location);
			if (itr_fileame == itr->second.getIndex().end())
			{
				std::cout << "indexfile bestaat niet dus autoindex" << std::endl;
				exit(1);
			}
			filename = itr->second.getRoot() + request_location + *itr_fileame;
		}
		else
		{
			filename = itr->second.getRoot() + request_location;
			if (!doesFileExist(filename))
			{
				std::cout << "file does not exits" << std::endl;
				exit(1);
			}
		}
		std::cout << "FILENAME: " << filename << std::endl;
		Response response = Response(filename, server);
		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
		_request = Request(); // waarom doen we dit
	}
	else if (_request.getType() == POST)
	{

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