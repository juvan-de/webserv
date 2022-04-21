#include <iostream> // cout
#include <unistd.h> // sleep
#include <csignal> // signal for ignoring pipe breaks
// custom
#include <connection.hpp> // handle_connection
#include <defines.hpp> // data struct, client struct

int main(int ac, char **av)
{
	if (ac == 2)
	{
		t_data					data;

		initialize_data(av[1], data);
		while (true)
		{
			for (std::vector<t_client>::iterator it = data.clients.begin(); it != data.clients.end(); it++)
				std::cout << "prepoll: " << it->fd << std::endl;
			poll(&data.fds[0], data.fds.size(), -1);
			handle_connection(data);
			std::cout << "Waiting for connections..." << std::endl;
			check_connection(data);
			sleep(1);
		}
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
