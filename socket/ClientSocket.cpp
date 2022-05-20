#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <Request.hpp>
#include <utils.hpp>
#include <sstream>

ClientSocket::ClientSocket(int fd, sockaddr_in addr) :
	Socket(fd), _request(Request()), _cgi(NULL)
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
	try
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
			std::cout << "CHUNKED" << std::endl;
			this->_request.readChunked(getFd());
		}
		if (this->_request.getInput().size() > 0)
		{
			this->_request.append_body();
		} 
		std::cout << std::endl << std::endl << "REQUEST" << std::endl << this->_request << std::endl;
		/* code */
	}
	catch(Request::RequestException& e)
	{
		this->_request.setType(ERROR);
		this->_request.setStatusCode(e.getError());
		this->_request.setAsFinished();
	}
}

/*----------------------------------------POLLOUT--------------------------------------------*/

Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request& request)
{
	std::map<std::string, std::string, cmpCaseInsensitive> headers = request.getHeaders();
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

	std::cout << "RESPONSE BUILDING" << std::endl;
	std::cout << location->first << std::endl;
	if (this->_request.getBody().size() > location->second.getClientMaxBodySize())
		return Response(413, server);
	if (location == server->getLocations().end()) /* bad request */
		return Response(404, server);
	std::cout << location->second.getRedir().isSet() << std::endl;
	if (location->second.getRedir().isSet())
		return Response(location->second.getRedir().getLocation());
	if (location->second.getLimitExcept().find("GET") == location->second.getLimitExcept().end()) /* bad request (405 forbidden)*/
		return Response(405, server);
	if (request_location[request_location.size() - 1] == '/')
	{
		std::vector<std::string>::const_iterator itr_filename = location->second.getRightIndexFile(location->second.getRoot() + request_location);
		if (itr_filename != location->second.getIndex().end())
			return Response(server, location->second.getRoot() + request_location + *itr_filename);
		if (location->second.getAutoindex())
			return Response(server, location->second.getRoot() + request_location);
		else
			return Response(403, server);
	}
	if (!doesFileExist(location->second.getRoot() + request_location))
		return Response(404, server);
	else
		return Response(server, location->second.getRoot() + request_location);
}


void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
{
	std::cout << "POLLOUT" << std::endl;
	if (this->_request.readyForParse()) //this is now a hacky solution
	{
		Response response;
		Server *server = find_server(table, this->_request);
		std::cout << "request in pollout:\n" << this->_request << std::endl;
		try 
		{
			if (!_cgi && (_request.getLocation().find(".php?") != std::string::npos || _request.getLocation().find(".py?") != std::string::npos))
				_cgi = new CgiSocket(_request, *server, _address);
			if (_cgi && _cgi->getStatus() == FINISHED)
			{
				this->_cgi->checkError();
				response = Response(this->_cgi->getInput(), true);
			}
		}
		catch (CgiSocket::CgiException& e)
		{
			response = Response(e.getError(), server);
		}
		if (this->_request.getType() == GET)
		{
			std::string	request_location = this->_request.getLocation();
			std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
			response = this->makeGetResponse(server, itr);
		}
		else if (this->_request.getType() == POST)
		{
			std::cout << "here we are going to upload the picture :D" << std::endl;
		}
		else if (this->_request.getType() == DELETE)
		{
			std::cout << "we be deletin tho" << std::endl;
		}
		else if (this->_request.getType() == ERROR)
		{
			std::cout << "TYPE ERROR" << std::endl;
			response = Response(this->_request.getStatusCode());
		}
		else
			std::cout << "nothing to do here" << std::endl;
		if (response.isFinished())
		{
			int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);//ik denk dat dit erbuiten moet gaan komen
			if (ret < 0)
				std::cout << "send error" << std::endl;
			this->_request = Request();	
		}
	}
}
