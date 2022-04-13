#include <Server.hpp>
#include <Request.hpp>
#include <Response.hpp>
#include <defines.hpp>
#include <sys/stat.h>

Server	*find_server(std::map<std::pair<int, std::string>, Server*>& table, Request request)
{
	std::map<std::string, std::string> headers = request.getHeaders();
	if (headers.find("Host") == headers.end())
	{
		/* bad request statuscode, want host is mandatory in http 1.1 */
		std::cout << "error finding hostname in headers:" << std::endl;
		std::cout << "-Req name loc ["<< request.getLocation() << "], req " << GET << "type: " << request.getType() << std::endl;
		for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
			std::cout << "--Header [" << it->first << "], Value [" << it->second << "]" << std::endl;
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
	struct stat	buf;
	
	std::string res = loc.getTitle() + loc.getRoot();
	for (std::vector<std::string>::const_iterator itr = loc.getIndex().begin(); itr != loc.getIndex().end(); itr++)
	{
		std::string filename = res + *itr;
		if (stat(filename.c_str(), &buf))
			return filename;
	}
	/* bad request */
	return NULL;
}