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

void	handle_response(t_client client, t_data data)
{
	Server *server = find_server(data.table, client.request);
	if (client.request.getType() == GET)
	{
		/* for now */
		std::map<std::string, Location>::const_iterator itr = find_right_location(server->getLocations(), client.request.getLocation());
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
		
		std::cout << "whooo do the post thiing" << std::endl;
	}
	else if(client.request.getType() == DELETE)
	{
		std::cout << "we be deletin tho" << std::endl;
	}
	else
	{
		std::cout << "shit went wrong yo" << std::endl;
	}
}
