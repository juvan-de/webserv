#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <utils.hpp>

ClientSocket::ClientSocket(int fd, sockaddr_in addr) :
	Socket(fd), _cgi(NULL), _hasCgi(false), _status(200), _request(Request())
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
	std::cout << "POLLIN FD: " << this->getFd() << std::endl;
	this->_request.addto_request(getFd());
	if (this->_request.getType() == NOTSET)
	{
		this->_request.setRequest();
		this->_request.setHeaders();
	}
	if (this->_request.checkIfChunked())
	{
		// std::cout << "CHUNKED" << std::endl;
		this->_request.readChunked(getFd());
	}
}

/*----------------------------------------POLLOUT--------------------------------------------*/

Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request& request)
{
	std::map<std::string, std::string> headers = request.getHeaders();
	// std::cout << request << std::endl;
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding hostname: " << std::endl;
	}
	std::string host = headers["Host"];
	std::string name = host.substr(0, host.find(":"));
	int port = std::atoi(host.substr(host.find(":") + 1).c_str());
	if (table.find(std::make_pair(port, name)) == table.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << " pair" << std::endl;
		return NULL;
	}
	return (table[std::make_pair(port, name)]);
}

Response ClientSocket::makeGetResponse(Server* server, std::map<std::string, Location>::const_iterator location)
{
	const std::string& request_location = this->_request.getLocation();

	if (location == server->getLocations().end()) /* bad request */
		return Response("", 404, server);
	if (location->second.getLimitExcept().find("GET") == location->second.getLimitExcept().end()) /* bad request (405 forbidden)*/
		return Response("", 405, server);
	if (request_location[request_location.size() - 1] == '/')
	{
		std::vector<std::string>::const_iterator itr_filename = location->second.getRightIndexFile(location->second.getRoot() + request_location);
		if (itr_filename != location->second.getIndex().end())
			return Response(location->second.getRoot() + request_location + *itr_filename, 200, server);
		if (location->second.getAutoindex())
			return Response(location->second.getRoot() + request_location, 200, server);
		else
			return Response("", 404, server);
	}
	if (!doesFileExist(location->second.getRoot() + request_location))
		return Response("", 404, server);
	else
		return Response(location->second.getRoot() + request_location, 200, server);
}

#include <Cgi.hpp>

void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table, Poller &poll)
{
	if (this->_request.readyForParse())
	{
		Response response;
		Server *server = find_server(table, this->_request);
		if (this->_request.readyForParse()) //very hacky solution, needs to be looked at
		{
			std::string	request_location = this->_request.getLocation();
			std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
			response = this->makeGetResponse(server, itr);
			int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
			this->_request = Request(); // waarom doen we dit
		}
		else if (this->_request.getType() == POST)
		{
			std::cout << "Post request" << std::endl;
			std::cout << this->_request.getLocation() << std::endl;
		}
		else if (this->_request.getType() == DELETE)
		{
			std::cout << "we be deletin tho" << std::endl;
		}
		else if (this->_request.getType() == ERROR)
		{
			std::cout << "shit went wrong yo" << std::endl;
		}
		else
		{
			std::cout << "nothing to do here" << std::endl;
		}
		if (_request.getLocation().find(".php?") != std::string::npos || _request.getLocation().find(".py?") != std::string::npos)
		{
			Cgi cgi = Cgi(_request, *server, _address);
			// std::cout << cgi;
		}
	}
}
