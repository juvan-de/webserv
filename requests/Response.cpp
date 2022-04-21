                                                                        
#include <Response.hpp>
#include <sstream>

Response::Response()
{
	// std::cout << "default constructor called" << std::endl;
}

// Response::Response(std::string error)
// {
// 	std::cout << "an error ocurred in Response constructor" << std::endl;
// }

Response::Response(std::string file, Server* server)
{
	StatusCodes statusCodes;
	std::stringstream ss;

	this->_path = server->getLocation("/").getRoot() + "/" + *server->getLocation("/").getIndex().begin();
	setResponseBody(this->_path);
	this->_statusCode = statusCodes.getStatusCode(200);
	ss << "HTTP/1.1 " << this->_statusCode.first << ' ' << this->_statusCode.second << "\r\n";
	ss << "Server: " << *(server->getServerName().begin()) << "\r\n";
	ss << "Content-length: " << getResponseBody().size() << "\r\n";
	ss << "Content-type: " << this->getRightContentType(file.substr(file.find_last_of(".") + 1)) << "\r\n"; //nog wel hardcode
	ss << "Connection: Keep-Alive" << "\r\n\r\n";
	ss << getResponseBody();
	this->_response = ss.str();
}

Response::Response(const Response& ref)
{
	*this = ref;
}

Response&	Response::operator=(const Response& ref)
{
	this->_statusCode = ref._statusCode;
	this->_path = ref._path;
	this->_response = ref._response;
	this->_responseBody = ref._responseBody;
	this->_contentTypes = ref._contentTypes;
	return (*this);
}

Response::~Response() {}

const std::string	&Response::getPath() const
{
	return (this->_path);
}

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
	this->_contentTypes["png"] = "image";
	this->_contentTypes["tiff"] = "image";
	this->_contentTypes["vnd.microsoft.icon"] = "image";
	this->_contentTypes["x-icon"] = "image";
	this->_contentTypes["vnd.djvu"] = "image";
	this->_contentTypes["svg+xml"] = "image";

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

const std::pair<int, std::string>	&Response::getStatusCode() const
{
	return (this->_statusCode);
}

const std::string					&Response::getResponse() const
{
	return (this->_response);
}

const std::string					&Response::getResponseBody() const
{
	return (this->_responseBody);
}

void		Response::setResponseBody(std::string &filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;

<<<<<<< HEAD
=======
	std::cout << "(DEBUG setResponseBody) filename: " << filename << std::endl;
>>>>>>> origin/juvan-de0.2
	if (file.is_open())
	{
		while (getline(file, line))
		{
			this->_responseBody.append(line);
		}
	}
	else
		throw NotAFile();
}

std::ostream&	operator<<(std::ostream &out, const Response &obj)
{
	out << "Response path:\n" << obj.getPath() << "\nStatus code:\n[" << obj.getStatusCode().first << "]" << std::endl;
	return (out);
}
