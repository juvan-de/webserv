#include <connection.hpp>

void	remove_last_dir(std::string& request_loc)
{
	request_loc = request_loc.substr(0, request_loc.find_last_of("/"));
}

std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>& locations, std::string request_loc)
{
	while (true)
	{
		if (locations.find(request_loc) != locations.end())
			return locations.find(request_loc);
		remove_last_dir(request_loc);
		if (request_loc.empty())
			return (locations.find("/"));
	}
}

void	handle_response(t_client &client, t_data &data)
{
	// std::cout << "BEFORE FINDING THA SERVA\n";
	// std::cout << client.request << std::endl;
	Server *server = find_server(data.table, client.request);
	if (client.request.getType() == GET)
	{
		/* for now */
		std::map<std::string, Location>::const_iterator itr = server->getLocations().find(client.request.getLocation());
		if (itr == server->getLocations().end())
		{
			/* bad request */
			std::cout << "DEBUG HANDLE RESPONSE: location: [" << client.request.getLocation() << "]" <<std::endl;
			std::cout << "bad request" << std::endl;
			return ;
		}
		if (itr->second.getLimitExcept().find("GET") == itr->second.getLimitExcept().end())
		{
			/* bad request (405 forbidden)*/
			std::cout << "bad request (forbidden)" << std::endl;
			return ;
		}
		std::string filename = getFileName(itr->second);
		Response response = Response(filename, server);
		int ret = send(client.fd, response.getResponse().c_str(), response.getResponse().length(), 0);
		// std::cout << "----------\n" << response.getResponseBody() << "\n----------" << std::endl;

		// get html locatie
		// formuleer een response header
		// add de html aan de body
		// verstuur naar client
		
	}
	else if (client.request.getType() == POST)
	{

	}
	else if (client.request.getType() == DELETE)
	{
		std::cout << "we be deletin tho" << std::endl;
	}
	else
	{
		std::cout << "shit went wrong yo" << std::endl;
	}
}

// std::string	basicResponse()
// {
// 	std::string ret;

// 	ret += "HTTP/1.1 200 ok\r\n";
// 	ret += "Server: localhost\r\n";
// 	ret += "Content-length: 82\r\n";
// 	ret += "Content-type: text/html\r\n";
// 	ret += "Connection: Keep-Alive\r\n\r\n";
// 	ret += "<html><body><h1>My First Heading echt</h1><p>My first paragraph.</p></body></html>";

// 	return ret;
// }

// void	handle_response(t_client client, t_data data)
// {
// 	Server *server = find_server(data.table, client.request);
// 	send(client.fd, basicResponse().c_str(), basicResponse().length(), 0);
// }
