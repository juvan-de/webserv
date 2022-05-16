#include <Response.hpp>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>

Response::Response()
{
	// std::cout << "default constructor called" << std::endl;
}

Response::Response(const Server *server, const std::string path)
{
	std::stringstream ss;
	StatusCodes statusCodes;
	std::string	contentType;

	if (path[path.size() - 1] == '/')
	{
		this->setResponseBodyFromDir(path);
		contentType = "text/html";
	}
	else
	{
		this->setResponseBodyFromFile(path);
		this->_setContentTypes();
		contentType = this->getRightContentType(path.substr(path.find_last_of(".") + 1));
	}

	ss << "HTTP/1.1 " << 200 << ' ' << "OK" << "\r\n"; //misschien veranderen zodat ie het toch uit de "data" haalt zodat het maar op1 plek staat
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: " << contentType << "\r\n\r\n";
	ss << this->_responseBody << "\r\n\r\n";
	this->_response = ss.str();
}

Response::Response(const Server *server, int errorcode)
{
	std::stringstream ss;
	StatusCodes statusCodes;
	setResponseBodyFromError(errorcode, server->getErrorPages());

	const std::pair<int, std::string>& statuscode = statusCodes.getStatusCode(errorcode);
	ss << "HTTP/1.1 " << statuscode.first << ' ' << statuscode.second << "\r\n";
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: " << "text/html" << "\r\n\r\n";
	ss << this->_responseBody << "\r\n\r\n";
	this->_response = ss.str();
}

Response::Response(const std::string& redir)
{
	std::stringstream ss;
	ss << "HTTP/1.1 " << 301 << ' ' << "Moved Permanently" << "\r\n"; //misschien veranderen zodat ie het toch uit de "data" haalt zodat het maar op1 plek staat
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
	this->_contentTypes = ref._contentTypes;
	return (*this);
}

Response::~Response() {}

void	Response::_setContentTypes()
{
	this->_contentTypes["EDI-X12"] = "application";
	this->_contentTypes["EDIFACT"] = "application";
	this->_contentTypes["javascript"] = "application";
	this->_contentTypes["octet-stream"] = "application";
	this->_contentTypes["ogg"] = "application";
	this->_contentTypes["pdf"] = "application";
	this->_contentTypes["xhtml+xml"] = "application";
	this->_contentTypes["x-shockwave-flash"] = "application";
	this->_contentTypes["json"] = "application";
	this->_contentTypes["ld+json"] = "application";
	this->_contentTypes["xml"] = "application";
	this->_contentTypes["zip"] = "application";
	this->_contentTypes["x-www-form-urlencoded"] = "application";
	
	this->_contentTypes["mpeg"] = "audio";
	this->_contentTypes["x-ms-wma"] = "audio";
	this->_contentTypes["vnd.rn-realaudio"] = "audio";
	this->_contentTypes["x-wav"] = "audio";
	
	this->_contentTypes["gif"] = "image";
	this->_contentTypes["jpeg"] = "image";
	this->_contentTypes["jpg"] = "image";
	this->_contentTypes["png"] = "image";
	this->_contentTypes["tiff"] = "image";
	this->_contentTypes["vnd.microsoft.icon"] = "image";
	this->_contentTypes["x-icon"] = "image";
	this->_contentTypes["vnd.djvu"] = "image";
	this->_contentTypes["svg+xml"] = "image";
	this->_contentTypes["ico"] = "image";

	this->_contentTypes["mixed"] = "multipart";
	this->_contentTypes["alternative"] = "multipart";
	this->_contentTypes["related"] = "multipart";
	this->_contentTypes["form-data"] = "multipart";

	this->_contentTypes["css"] = "text";
	this->_contentTypes["csv"] = "text";
	this->_contentTypes["html"] = "text";
	this->_contentTypes["javascript"] = "text";
	this->_contentTypes["plain"] = "text";
	this->_contentTypes["xml"] = "text";
	
	this->_contentTypes["mpeg"] = "video";
	this->_contentTypes["mp4"] = "video";
	this->_contentTypes["quicktime"] = "video";
	this->_contentTypes["x-ms-wmv"] = "video";
	this->_contentTypes["x-msvideo"] = "video";
	this->_contentTypes["x-flv"] = "video";
	this->_contentTypes["webm"] = "video";
}

const std::string	Response::getRightContentType(const std::string suffix) const
{
	std::map<std::string, std::string>::const_iterator itr = this->_contentTypes.find(suffix);
	if (itr != this->_contentTypes.end())
		return itr->second + "/" + itr->first; 
	/* error, wat willen we dan*/
	std::cout << "error, wat willen we dan" << std::endl;
	return (NULL);
}

const std::string					&Response::getResponse() const
{
	return (this->_response);
}

const std::string					&Response::getResponseBody() const
{
	return (this->_responseBody);
}

void		Response::setResponseBodyFromDir(const std::string &dirname)
{
	DIR*			dir;
	struct dirent*	cur_file;

	this->_responseBody = "<html><head><title>Index of " + dirname + "</title></head>";
	this->_responseBody += "<body><h1>Index of " + dirname + "</h1><hr><pre>";
	dir = opendir(dirname.c_str());
	if (dir != NULL)
	{
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
	}
	this->_responseBody += "</pre><hr></body></html>";
}

void		Response::setResponseBodyFromFile(const std::string &filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;
	std::ostringstream Stream;

	if (file.is_open())
	{
		// this->_responseBody.append(Stream.str());
		Stream << file.rdbuf();
		this->_responseBody = Stream.str();
	}
	else
		throw NotAFile();
}

void		Response::_makeDefaultErrorPage(std::pair<int, std::string> errcode, std::ostringstream& Stream)
{
	std::string defaultError = "files/html/Website/Error/error.html";
	std::ifstream file(defaultError.c_str());
	Stream << file.rdbuf();
	this->_responseBody = Stream.str();
	size_t replacable = _responseBody.find("{{ERROR_CODE}}");
	this->_responseBody.replace(replacable, 14, std::to_string(errcode.first));
	replacable = _responseBody.find("{{ERROR_MESSAGE}}");
	this->_responseBody.replace(replacable, 17, errcode.second);
}

void		Response::setResponseBodyFromError(int code, const std::map<int, std::string>& errorPages)
{
	std::map<int, std::string>::const_iterator itr = errorPages.find(code);
	std::ostringstream Stream;
	StatusCodes codes;
	std::pair<int, std::string> errcode = codes.getStatusCode(code);
	if (itr != errorPages.end())
	{
		std::ifstream	file(itr->second.c_str());
		if (file) // dit klopt niet volgensmij, als de file niet bestaat moet je 404 opgeven toch?
		{
			Stream << file.rdbuf();
			this->_responseBody = Stream.str();
		}
		else
			_makeDefaultErrorPage(errcode, Stream);
	}
	else
		_makeDefaultErrorPage(errcode, Stream);
}

std::ostream&	operator<<(std::ostream &out, const Response &obj)
{
	out << "Response:\n" << obj.getResponse();
	return (out);
}
