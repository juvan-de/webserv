#include <iostream> // cout
#include <unistd.h> // sleep
// custom
#include <connection.hpp> // handle_connection
#include <defines.hpp> // data struct, client struct

int main(int ac, char **av)
{
	if (ac == 2)
	{
		t_data					data;
		std::vector<t_client>	clients;

		initialize_data(av[1], data);
		std::cout << "Starting server" << std::endl;;
		while (true)
		{
			for (int i = 0; i < data.socket_num; i++)
				data.fds[i].events = POLLIN;
			poll(&data.fds[0], data.fds.size(), -1);
			// handle_connection(data.servers_configs, clients);
			// if (clients.size())
			// 	std::cout << "new cli main: " << clients.back()->fd.events << ", " << POLLIN << std::endl;
			for (std::vector<t_client>::iterator it = clients.begin(); it != clients.end(); it++)
				std::cout << "cli fd: " << it->fd->fd << ", revent: " << it->fd->revents << std::endl;
			// for (std::vector<pollfd>::iterator it = clients.begin(); it != clients.end(); it++)
			// 	std::cout << "cli fd: " << it->fd->fd << ", revent: " << it->fd->revents << std::endl;
			std::cout << "Waiting for connections..." << std::endl;
			for (int i = 0; i < data.socket_num; i++)
				check_connection(data, clients, i);
			sleep(1);
		}
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
