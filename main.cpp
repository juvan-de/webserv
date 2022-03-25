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

		initialize_data(av[1], data);
		while (true)
		{
			for (int i = 0; i < data.socket_num; i++)
				data.fds[i].events = POLLIN;
			// for (std::vector<t_client>::iterator it = clients.begin(); it != clients.end(); it++)
			// 	std::cout << "prepoll: " << it->fd.fd << "\trevent: " << it->fd.revents << "\t" << POLLIN << "\t" << POLLOUT << std::endl;
			poll(&data.fds[0], data.fds.size(), -1);
			handle_connection(data);
			// for (std::vector<t_client>::iterator it = clients.begin(); it != clients.end(); it++)
			// 	std::cout << "postpoll: " << it->fd.fd << "\trevent: " << it->fd.revents << std::endl;
			std::cout << "Waiting for connections..." << std::endl;
			check_connection(data);
			sleep(1);
		}
	}
	else
		std::cout << "incorrect arguments" << std::endl;
	return (0);	
}
