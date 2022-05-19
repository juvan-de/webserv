#include "ContentTypes.hpp"
#include <iostream>

ContentTypes::ContentTypes()
{
	std::cout << "STATUSCODE DEFAULT CNSTRUCTOR CALLED!!!" << std::endl;
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

const std::string&	ContentTypes::IContentType(const std::string& key)
{
	return this->_contentTypes.find(key)->second;
}