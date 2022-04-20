#include <Socket.hpp>
#include <unistd.h> //close
#include <vector>
#include <utils.hpp> // newPoll
#include <errno.h>
/*--------------------------------Coplien form--------------------------------*/
Socket::Socket()
{
	/*Constructor*/
	return ;
}

Socket::~Socket()
{
	/*Destructor*/
	// std::cout << "Debug: closing server_sock " << _port << std::endl;
	// close(_servfd);
}

Socket::Socket(const Socket &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Socket&	Socket::operator=(const Socket &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		_fd = ref._fd;
		_address = ref._address;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	// Set address struct
	bzero(&_address, sizeof(_address));
	_address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = interface;

    // Establish a connection
	if ((_fd = socket(domain, service, protocol)) < 0)
		throw badInit;
}

int	Socket::new_connection(sockaddr *cli_addr)
{
	socklen_t len;
	len = sizeof(cli_addr);

	return accept(_fd, (struct sockaddr *)cli_addr, &len);
}

/*------------------------------Other connection funcs------------------------------*/

t_client	accept_client(int fd, sockaddr &addr)
{
	t_client new_client;
	int opt = 1;

	bzero(&new_client, sizeof(new_client));
	new_client.opt = 1;
	new_client.fd = fd;
	new_client.addr = addr;
	std::cout << "TEST: " << new_client.addr.sa_len << std::endl;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &new_client.opt, sizeof(new_client.opt));
	new_client.request = Request();
	// beter als we hier een define gebruiken
	new_client.status = 200;
	std::cout << "> DEBUG: Connected!: " << fd << std::endl;
	return new_client;
}

void	check_connection(t_data &data)
{
	for (int i = 0; i < data.socket_num; i++)
	{
		if (data.fds[i].revents & POLLIN)
		{
			int cli_sock;
			sockaddr cli_addr;

			if ((cli_sock = data.sockets[i].new_connection(&cli_addr)) < 0)
			{
				if (errno != EWOULDBLOCK)
					std::cout << "error occured" << std::endl;
			}
			else
			{
				data.clients.push_back(accept_client(cli_sock, cli_addr));
				data.fds.push_back(new_pollfd(cli_sock)); // error protection needed
			}
			if (errno)
				std::cout << "ERRNO in check connection: " << errno << std::endl;
		}

	}
}
