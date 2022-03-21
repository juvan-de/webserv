#include <iostream> // cout
#include <unistd.h> // sleep
// custom
#include <connection.hpp> // handle_connection
#include <defines.hpp> // data struct, client struct

void	set_client_fds(std::vector<t_client> &clients, std::vector<pollfd> &fds)
{
	for(std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		for (std::vector<t_client>::iterator itc = clients.begin(); itc != clients.end(); itc++)
		{
			if (itc->fd.fd == it->fd)
			{
				itc->fd.events = it->events;
				itc->fd.revents = it->revents;
				break ;
			}
		}
	}
}

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
			// for (std::vector<t_client>::iterator it = clients.begin(); it != clients.end(); it++)
			// 	std::cout << "prepoll: " << it->fd.fd << "\trevent: " << it->fd.revents << "\t" << POLLIN << "\t" << POLLOUT << std::endl;
			poll(&data.fds[0], data.fds.size(), -1);
			set_client_fds(clients, data.fds);
			handle_connection(data.server_configs, clients);
			// for (std::vector<t_client>::iterator it = clients.begin(); it != clients.end(); it++)
			// 	std::cout << "postpoll: " << it->fd.fd << "\trevent: " << it->fd.revents << std::endl;
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
