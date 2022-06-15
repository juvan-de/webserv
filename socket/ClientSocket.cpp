#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <Request.hpp>
#include <utils.hpp>
#include <sstream>
#include <ostream>
#include <stdio.h>

ClientSocket::ClientSocket(int fd, sockaddr_in addr, int serverPort) :
	Socket(fd), _request(Request()), _server(NULL), _cgi(NULL), _serverPort(serverPort)
{
	// std::cout << "DEBUG: CLI SOCK OPENED" << std::endl;
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

void	ClientSocket::handle_pollin(std::map<std::pair<int, std::string>, Server*>& table)
{
	try
	{
		this->_request.addto_request(getFd());
		if (this->_request.getType() == NOTSET)
		{
			this->_request.setRequest();
			this->_request.setHeaders();
			this->_server = find_server(table, _request);
		}
		if (this->_request.checkIfChunked())
		{
			this->_request.readChunked(getFd());
		}
		if (this->_request.getInput().size() > 0)
		{
			this->_request.append_body();
		}
		if (this->_request.isFinished())
			this->check_cgi();
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

Server	*ClientSocket::find_server(std::map<std::pair<int, std::string>, Server*>& table, Request& request)
{
	std::map<std::string, std::string, cmpCaseInsensitive> headers = request.getHeaders();
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
	}
	std::string host = headers["Host"];
	std::string name = host.substr(0, host.find(":"));
	int port = this->getPort();
	if (table.find(std::make_pair(port, name)) == table.end())
	{
		std::cout << "this shouldn't happen :C" << std::endl;
		return NULL;
	}
	return (table[std::make_pair(port, name)]);
}

Response ClientSocket::handle_get(std::map<std::string, Location>::const_iterator location)
{
	const std::string& uri = this->_request.getUri();

	// std::cout << "RESPONSE BUILDING" << std::endl;
	if (this->_request.getBody().size() > location->second.getClientMaxBodySize())
		return Response(413, this->_server);
	if (location == this->_server->getLocations().end()) /* bad request */
		return Response(404, this->_server);
	if (location->second.getRedir().isSet())
	{
		std::cout << "redirect goes wrong here" << std::endl;
		return Response(location->second.getRedir().getLocation());
	}
	if (location->second.getLimitExcept().find("GET") == location->second.getLimitExcept().end()) /* bad request (405 forbidden)*/
		return Response(405, this->_server);
	if (uri[uri.size() - 1] == '/')
	{
		std::vector<std::string>::const_iterator itr_filename = location->second.getRightIndexFile(location->second.getRoot() + uri);
		if (itr_filename != location->second.getIndex().end())
			return Response(this->_server, location->second.getRoot() + uri + *itr_filename, location->second.getRoot());
		if (location->second.getAutoindex())
			return Response(this->_server, location->second.getRoot() + uri, location->second.getRoot());
		else
			return Response(403, this->_server);
	}
	if (!doesFileExist(location->second.getRoot() + uri))
		return Response(404, this->_server);
	else
		return Response(this->_server, location->second.getRoot() + uri, location->second.getRoot());
}

Response	ClientSocket::handle_post(std::map<std::string, Location>::const_iterator location, const std::string& uri)
{
	if (uri.size() > 0 && uri[uri.size() - 1] == '/')
		return Response(400, this->_server);
	if (this->_request.getBody().size() > location->second.getClientMaxBodySize())
		return (Response(413, this->_server));
	if (location->second.getLimitExcept().find("POST") == location->second.getLimitExcept().end())
		return (Response(405, this->_server));
	std::string upload_location = location->second.getRoot() + this->_request.getUri();
	std::ofstream os(upload_location, std::ofstream::binary);
	os.write(this->_request.getBody().c_str(), this->_request.getBody().size());
	return (Response(200, this->_server));
}

Response	ClientSocket::handle_delete(std::map<std::string, Location>::const_iterator location)
{
	std::string delete_location = location->second.getRoot() + this->_request.getUri();
	if (location->second.getLimitExcept().find("DELETE") == location->second.getLimitExcept().end())
		return (Response(405, this->_server));
	if (!doesFileExist(delete_location))
		return (Response(404, this->_server));
	int ret = std::remove(delete_location.c_str());
	if (ret < 0)
		return (Response(500, this->_server));
	return (Response(200, this->_server));
}

Response	ClientSocket::handle_cgi()
{
	Response response = Response();

	if (_cgi->getStatus() == FINISHED)
	{
		std::cout << "body: " << _cgi->getOutput() << std::endl;
		// try
		// {
		// 	this->_cgi->checkError();/* code */
		// }
		// catch(const std::exception& e)
		// {
		// 	std::cerr << e.what() << '\n';
		// }
		this->_cgi->checkError();/* code */
		response = Response(this->_cgi->getOutput(), true);
		_cgi->setSatus(FINISHED);
		_cgi = NULL;
	}
	return response;
}

void	ClientSocket::check_cgi()
{
	std::string	request_location = this->_request.getUri();
	std::map<std::string, Location>::const_iterator location = this->_server->getRightLocation(request_location);

	if (location != this->_server->getLocations().end())
	{
		for (std::map<std::string, std::string>::const_iterator it = location->second.getCgi().begin(); it != location->second.getCgi().end(); it++)
		{
			//return cgi script name if it is found in allowed cgi types
			size_t pos;

			if (this->_request.getType() == POST)
			{
				if ((pos = request_location.find(it->first)) != std::string::npos)
					_cgi = new CgiSocket(request_location.substr(0, pos) + it->first, this->_request, *this->_server, this->_address);
			}
			else
			{
				if ((pos = request_location.find(it->first + "?")) != std::string::npos)
					_cgi = new CgiSocket(request_location.substr(0, pos) + it->first, this->_request, *this->_server, this->_address);
			}
		}
	}
}

Response ClientSocket::makeResponse()
{
	if (this->_request.getType() == ERROR)
		return Response(this->_request.getStatusCode());
	const std::string	uri = this->_request.getUri();
	std::map<std::string, Location>::const_iterator itr = this->_server->getRightLocation(uri);
	if (_cgi)
		return this->handle_cgi();
	else if (this->_request.getType() == GET)
		return this->handle_get(itr);
	else if (this->_request.getType() == POST)
		return handle_post(itr, uri);
	else if (this->_request.getType() == DELETE)
		return handle_delete(itr);
	return Response(500);
}


void	ClientSocket::handle_pollout()
{
	Response response;

	response = this->makeResponse();
	if (response.isFinished())
	{
		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
		if (ret < 0)
			std::cout << "send error" << std::endl; //what we do
		this->_request = Request();	
	}
}
