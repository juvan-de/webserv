#include <Response.hpp>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>

Response::Response()
{
	// std::cout << "default constructor called" << std::endl;
}

Response::Response(const Server *server, const std::string& path, const std::string& root)
{
	std::stringstream ss;
	std::string	contentType;

	try
	{
		int ret = isValidPath(root, path);
		std::cout << "compare: " << ret << std::endl;
		if (ret != 0)
			throw ResponseException(ret);
		if (path[path.size() - 1] == '/')
		{
			this->setResponseBodyFromDir(path);
			contentType = "text/html";
		}
		else
		{
			this->setResponseBodyFromFile(path);
			contentType = this->getRightContentType(path.substr(path.find_last_of(".") + 1));
		}
	}
	catch(ResponseException &e)
	{
		*this = Response(e.getError(), server);
		return ;
	}
	ss << "HTTP/1.1 " << 200 << ' ' << "OK" << "\r\n";
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: " << contentType << "\r\n\r\n";
	ss << this->_responseBody << "\r\n\r\n";
	this->_response = ss.str();
}

Response::Response(int errorCode, const Server *server)
{
	std::stringstream ss;
	const std::string errorStatus = StatusCodes::getStatusCode(errorCode);
	if (errorStatus.empty())
		throw ResponseException(500);
	if (server)
		this->setResponseBodyFromError(errorCode, errorStatus, server->getErrorPages());
	else
		this->_makeDefaultErrorPage(errorCode, errorStatus);
	ss << "HTTP/1.1 " << errorCode << ' ' << errorStatus << "\r\n";
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: " << "text/html" << "\r\n\r\n";
	ss << this->_responseBody << "\r\n\r\n";
	this->_response = ss.str();
}

Response::Response(const std::string& redir)
{
	std::stringstream ss;
	ss << "HTTP/1.1 " << 301 << ' ' << "Moved Permanently" << "\r\n";
	ss << "Location: " << redir << "\r\n\r\n";
	this->_response = ss.str();
}

Response::Response(const Response& ref)
{
	*this = ref;
}

Response&	Response::operator=(const Response& ref)
{
	this->_response = ref._response;
	this->_responseBody = ref._responseBody;
	return (*this);
}

Response::~Response() {}

const std::string	Response::getRightContentType(const std::string suffix) const
{
	std::string prefix = ContentTypes::getContentType(suffix);
	if (prefix.empty())
		throw ResponseException(415);
	return (prefix + "/" + suffix);
}

const std::string					&Response::getResponse() const
{
	return (this->_response);
}

const std::string					&Response::getResponseBody() const
{
	return (this->_responseBody);
}

void		Response::setResponseBodyFromDir(const std::string &dirname)//errorcodes zijn gechecked
{
	DIR*			dir;
	struct dirent*	cur_file;

	dir = opendir(dirname.c_str());
	if (dir != NULL)
	{
		this->_responseBody = "<html><head><title>Index of " + dirname + "</title></head>";
		this->_responseBody += "<body><h1>Index of " + dirname + "</h1><hr><pre>";
		while ((cur_file = readdir(dir)))
		{
			if (strcmp(cur_file->d_name, ".") == 0)
				continue ;
			this->_responseBody += " <a href=\"";
			this->_responseBody += cur_file->d_name;
			this->_responseBody += "\">";
			this->_responseBody += cur_file->d_name;
			this->_responseBody += "</a><br>";
		}
		closedir(dir);
		this->_responseBody += "</pre><hr></body></html>";
	}
	else
		throw ResponseException(404);
}

void		Response::setResponseBodyFromFile(const std::string &filename)//errorcodes zijn gechecked
{
	std::ifstream	file(filename.c_str());
	std::string		line;
	std::ostringstream Stream;

	if (!(file.std::ios::fail()))
	{
		Stream << file.rdbuf();
		this->_responseBody = Stream.str();
	}
	else
		throw ResponseException(500);
}

void		Response::_makeDefaultErrorPage(int errorCode, const std::string& errorStatus)//errorcodes zijn gechecked
{
	std::string defaultError = "files/html/Website/Error/error.html";
	std::ifstream file(defaultError.c_str()); //nee hoeven we niet te checken want default is er altijd
	std::ostringstream	Stream;
	Stream << file.rdbuf();
	this->_responseBody = Stream.str();
	size_t replacable = _responseBody.find("{{ERROR_CODE}}");
	this->_responseBody.replace(replacable, 14, std::to_string(errorCode));
	replacable = _responseBody.find("{{ERROR_MESSAGE}}");
	this->_responseBody.replace(replacable, 17, errorStatus);
}

void		Response::setResponseBodyFromError(int code, const std::string& errorStatus, const std::map<int, std::string>& errorPages)//errorcodes zijn gechecked
{
	std::map<int, std::string>::const_iterator itr = errorPages.find(code);
	if (itr != errorPages.end())
	{
		std::ostringstream Stream;
		std::ifstream	file(itr->second.c_str());
		if (!(file.std::ios::fail()))
		{
			Stream << file.rdbuf();
			this->_responseBody = Stream.str();
		}
		else
			_makeDefaultErrorPage(404, errorStatus);
	}
	else
		_makeDefaultErrorPage(code, errorStatus);
}

std::ostream&	operator<<(std::ostream &out, const Response &obj)
{
	out << "Response:\n" << obj.getResponse();
	return (out);
}
