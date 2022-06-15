#include <ClientSocket.hpp>
#include <BadInit.hpp>
#include <Request.hpp>
#include <utils.hpp>
#include <sstream>
#include <ostream>
#include <stdio.h>

ClientSocket::ClientSocket(int fd, sockaddr_in addr, int serverPort) :
	Socket(fd), _request(Request()), _cgi(NULL), _serverPort(serverPort)
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

void	ClientSocket::handle_pollin()
{
	try
	{
		this->_request.addto_request(getFd());
		if (this->_request.getType() == NOTSET)
		{
			this->_request.setRequest();
			this->_request.setHeaders();
		}
		if (this->_request.checkIfChunked())
		{
			this->_request.readChunked(getFd());
		}
		if (this->_request.getInput().size() > 0)
		{
			this->_request.append_body();
		}
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

Response ClientSocket::handle_get(Server* server, std::map<std::string, Location>::const_iterator location)
{
	const std::string& uri = this->_request.getUri();

	// std::cout << "RESPONSE BUILDING" << std::endl;
	if (this->_request.getBody().size() > location->second.getClientMaxBodySize())
		return Response(413, server);
	if (location == server->getLocations().end()) /* bad request */
		return Response(404, server);
	if (location->second.getRedir().isSet())
	{
		std::cout << "redirect goes wrong here" << std::endl;
		return Response(location->second.getRedir().getLocation());
	}
	if (location->second.getLimitExcept().find("GET") == location->second.getLimitExcept().end()) /* bad request (405 forbidden)*/
		return Response(405, server);
	if (uri[uri.size() - 1] == '/')
	{
		std::vector<std::string>::const_iterator itr_filename = location->second.getRightIndexFile(location->second.getRoot() + uri);
		if (itr_filename != location->second.getIndex().end())
			return Response(server, location->second.getRoot() + uri + *itr_filename, location->second.getRoot());
		if (location->second.getAutoindex())
			return Response(server, location->second.getRoot() + uri, location->second.getRoot());
		else
			return Response(403, server);
	}
	if (!doesFileExist(location->second.getRoot() + uri))
		return Response(404, server);
	else
		return Response(server, location->second.getRoot() + uri, location->second.getRoot());
}

Response	ClientSocket::handle_post(Server* server, std::map<std::string, Location>::const_iterator location, const std::string& uri)
{
	if (uri.size() > 0 && uri[uri.size() - 1] == '/')
		return Response(400, server);
	if (this->_request.getBody().size() > location->second.getClientMaxBodySize())
		return (Response(413, server));
	if (location->second.getLimitExcept().find("POST") == location->second.getLimitExcept().end())
		return (Response(405, server));
	std::string upload_location = location->second.getRoot() + this->_request.getUri();
	std::ofstream os(upload_location, std::ofstream::binary);
	os.write(this->_request.getBody().c_str(), this->_request.getBody().size());
	return (Response(200, server));
}

Response	ClientSocket::handle_delete(Server* server, std::map<std::string, Location>::const_iterator location)
{
	std::string delete_location = location->second.getRoot() + this->_request.getUri();
	if (location->second.getLimitExcept().find("DELETE") == location->second.getLimitExcept().end())
		return (Response(405, server));
	if (!doesFileExist(delete_location))
		return (Response(404, server));
	int ret = std::remove(delete_location.c_str());
	if (ret < 0)
		return (Response(500, server));
	return (Response(200, server));
}

// static Response	handle_cgi()
// {
// 	if (_cgi->getStatus() == FINISHED)
// 	{
// 		_cgi->checkError();
// 		response = Response(_cgi->getInput(), true);
// 		_cgi->setSatus(SENT);
// 		_cgi = NULL;
// 	}
// }

static std::string	isCgiRequest(Server *server, Request request)
{
	std::string	request_location = request.getUri();
	std::map<std::string, Location>::const_iterator location = server->getRightLocation(request_location);

	if (location == server->getLocations().end())
		return "";
	else
	{
		for (std::map<std::string, std::string>::const_iterator it = location->second.getCgi().begin(); it != location->second.getCgi().end(); it++)
		{
			size_t pos;
			if (request.getType() == POST)
			{
				if ((pos = request_location.find(it->first)) != std::string::npos)
					return request_location.substr(0, pos) + it->first;
			}
			else
			{
				if ((pos = request_location.find(it->first + "?")) != std::string::npos)
					return request_location.substr(0, pos) + it->first;
			}
		}
	}
	return "";
}

// void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
// {
// 	Response response;
// 	Server *server = find_server(table, this->_request);
// 	std::string	filename = isCgiRequest(server, this->_request);
// 	std::string	request_location = this->_request.getUri();
// 	std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);

// 	try 
// 	{
// 		if (!_cgi && filename.compare(""))
// 			_cgi = new CgiSocket(filename, this->_request, *server, _address);
// 	}
// 	catch (CgiSocket::CgiException& e)
// 	{
// 		response = Response(e.getError(), server);
// 	}

	
// 	switch (this->_request.getType())
// 	{
// 		case NOTSET:
// 			break;	
// 		case ERROR:
// 			response = Response(this->_request.getStatusCode());
// 			break;
// 		case CGI:
// 			handle_cgi(_request);
// 			break;
// 		case GET:
// 			response = handle_get(server, itr);
// 			break;
// 		case POST:
// 			response = handle_post(server, itr);
// 			break;
// 		case DELETE:
// 			response = handle_delete(server, itr);
// 			break;
// 	}
// 	if (response.isFinished())
// 	{
// 		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
// 		if (ret < 0)
// 			std::cout << "send error" << std::endl;
// 		this->_request = Request();	
// 	}
// }

Response ClientSocket::makeResponse(Server* server)
{
	if (this->_request.getType() == ERROR)
		return Response(this->_request.getStatusCode());
	const std::string	uri = this->_request.getUri();
	std::map<std::string, Location>::const_iterator itr = server->getRightLocation(uri);
	if (this->_request.getType() == GET)
		return this->handle_get(server, itr);
	else if (this->_request.getType() == POST)
		return handle_post(server, itr, uri);
	else if (this->_request.getType() == DELETE)
		return handle_delete(server, itr);
	return NULL;
}


void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
{
	Response response;
	std::string	filename;
	Server *server = NULL;

	if (this->_request.getType() != ERROR)
	{
		server = find_server(table, this->_request);
		//something so it doesn't segfault when server couldn't be found
		try 
		{
			filename = isCgiRequest(server, this->_request);
			if (!_cgi && filename.compare(""))
				_cgi = new CgiSocket(filename, this->_request, *server, _address);
			if (_cgi && _cgi->getStatus() == FINISHED)
			{
				this->_cgi->checkError();
				response = Response(this->_cgi->getInput(), true);
				_cgi->setSatus(SENT);
				_cgi = NULL;
			}
			if (_cgi && _cgi->getStatus() != FINISHED)
				return ;
		}
		catch (CgiSocket::CgiException& e)
		{
			response = Response(e.getError(), server);
		}
	}
	response = this->makeResponse(server);
	if (response.isFinished())
	{
		int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
		if (ret < 0)
			std::cout << "send error" << std::endl; //what we do
		this->_request = Request();	
	}

}

// void	ClientSocket::handle_pollout(std::map<std::pair<int, std::string>, Server*>	table)
// {
// 	if (this->_request.readyForParse())
// 	{
// 		Response response;
// 		std::string	filename;
// 		Server *server = NULL;
// 		if (this->_request.getType() != ERROR)
// 		{
// 			server = find_server(table, this->_request);
// 			//something so it doesn't segfault when server couldn't be found
// 			try 
// 			{
// 				filename = isCgiRequest(server, this->_request);
// 				// std::cout << "DEBUG FILENAME: " << filename << ", compare ret: " << filename.compare("") << std::endl;
// 				if (!_cgi && filename.compare(""))
// 					_cgi = new CgiSocket(filename, this->_request, *server, _address);
// 				if (_cgi && _cgi->getStatus() == FINISHED)
// 				{
// 					this->_cgi->checkError();
// 					response = Response(this->_cgi->getInput(), true);
// 					_cgi->setSatus(SENT);
// 					_cgi = NULL;
// 				}
// 				if (_cgi && _cgi->getStatus() != FINISHED)
// 					return ;
// 			}
// 			catch (CgiSocket::CgiException& e)
// 			{
// 				response = Response(e.getError(), server);
// 			}
// 		}
// 		if (this->_request.getType() == GET)
// 		{
// 			std::string	request_location = this->_request.getUri();
// 			std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
// 			response = this->handle_get(server, itr);
// 		}
// 		else if (this->_request.getType() == POST)
// 		{
// 			std::string	request_location = this->_request.getUri();
// 			if (request_location.size() > 0 && request_location[request_location.size() - 1] == '/')
// 				response = Response(400, server);
// 			else
// 			{
// 				std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
// 				response = handle_post(server, itr);
// 			}
// 		}
// 		else if (this->_request.getType() == DELETE)
// 		{
// 			std::string	request_location = this->_request.getUri();
// 			std::map<std::string, Location>::const_iterator itr = server->getRightLocation(request_location);
// 			response = handle_delete(server, itr);
// 		}
// 		else if (this->_request.getType() == ERROR)
// 			response = Response(this->_request.getStatusCode());
// 		if (response.isFinished())
// 		{
// 			int ret = send(getFd(), response.getResponse().c_str(), response.getResponse().length(), 0);
// 			if (ret < 0)
// 				std::cout << "send error" << std::endl; //what we do
// 			this->_request = Request();	
// 		}
// 	}
// }
