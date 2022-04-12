#include <connection.hpp>

void	handle_response(t_client client, t_data data)
{
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
