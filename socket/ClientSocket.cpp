#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <sys/stat.h> // stat struct
#include <utils.hpp>

/*--------------------------------Coplien form--------------------------------*/
ClientSocket::~ClientSocket()
{
	/*Destructor*/
}

ClientSocket::ClientSocket(const ClientSocket &ref) : Socket(ref)
{
	*this = ref;
}

ClientSocket&	ClientSocket::operator=(const ClientSocket &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		_status = ref._status;
		_address = ref._address;
		_request = ref._request;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

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

void	ClientSocket::setCgi(int fd)
{
	
}

void			ClientSocket::setServer(std::map<std::pair<int, std::string>, Server*> table)
{
	std::map<std::string, std::string> headers = _request.getHeaders();
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding hostname" << std::endl;
		// throw error here
	}
	std::string host = headers["Host"];
	std::string name = host.substr(0, host.find(":"));
	int port = std::atoi(host.substr(host.find(":") + 1).c_str());
	if (table.find(std::make_pair(port, name)) == table.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding pair" << std::endl;
		// throw error here
	}
	_serv = table[std::make_pair(port, name)];
}

void	ClientSocket::handle_pollin(std::map<std::pair<int, std::string>, Server*> table)
{
	std::cout << "POLLING IN" << std::endl;
	_request.addto_request(getFd());
	if (_request.getType() == NOTSET)
	{
		_request.setRequest();
		_request.setHeaders();
		this->setServer(table);
	}
	if (_request.checkIfChunked())
	{
		// std::cout << "CHUNKED" << std::endl;
		_request.readChunked(getFd());
	}
}

/*----------------------------------------POLLOUT--------------------------------------------*/

void	ClientSocket::handle_pollout(Poller &poll)
{
	std::cout << "POLLING OUT" << std::endl;

	if (_request.getType() == GET)
	{
		std::string	filename;
		std::string	request_location = this->_request.getLocation();
		std::map<std::string, Location>::const_iterator itr = find_right_location(this->_serv->getLocations(), request_location);
		std::cout << "request_location: " << request_location << std::endl;
		if (itr == this->_serv->getLocations().end())
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
		if (itr->first == request_location)
		{
			if (itr->second.getAutoindex())
				filename = getFileName(itr->second);
			else
			{
				std::cout << "Index list?? Ingmar" << std::endl;
				exit(1);
			}
		}
		else
		{
			filename = itr->second.getRoot() + request_location;
			if (!doesFileExits(filename))
			{
				std::cout << "file does not exits" << std::endl;
				exit(1);
			}
		}
		std::cout << "FILENAME: " << filename << std::endl;
		Response response = Response(filename, this->_serv);
//		std::cout << response << std::endl;
		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
//		std::cout << ret << "\t" << response.getResponse().length() << std::endl;
		_request = Request();
	}
	else if (_request.getType() == POST)
	{
		std::cout << "Post request" << std::endl;
		std::cout << _request.getLocation() << std::endl;
		if (_request.getLocation().find(".php?") || _request.getLocation().find(".py?"))
			_cgi = Cgi();
	}
	else if (_request.getType() == DELETE)
	{
		std::cout << "we be deletin tho" << std::endl;
	}
	else
	{
		// std::cout << "shit went wrong yo" << std::endl;
	}
}
