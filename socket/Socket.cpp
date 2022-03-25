#include <Socket.hpp>
#include <unistd.h> //close
#include <vector>

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
		_port = ref._port;
		_servfd = ref._servfd;
		_address = ref._address;
		_opted = ref._opted;
		_address_len = ref._address_len;
		_flags = ref._flags;
		_backlog = ref._backlog;
		_poll = ref._poll;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Socket::Socket(int port) : _port(port)
{
	/*Constructor*/
	std::cout << "Initializing port" << _port << std::endl;

	/*Initializing sock address*/
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	_address_len = sizeof(_address);
	
	/*Initializing configurations*/
	_opted = 1;
	_backlog = 100;

	/*Instantiating a connection*/
	if ((_servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw badInit;
	if ((_flags = fcntl(_servfd, F_GETFL)) < 0)
		throw badInit;
	if (fcntl(_servfd, F_SETFL, _flags | O_NONBLOCK) < 0)
		throw badInit;
	if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &_opted, sizeof(_opted)) < 0)
		throw badInit;
	if ((bind(_servfd, (struct sockaddr *)&_address, sizeof(_address)) < 0))
		throw badInit;
	if (listen(_servfd, _backlog) < 0)
		throw badInit;

	/*Initializing pollfd struct*/
	_poll.fd = _servfd;
	_poll.events = POLLIN;
}

int	Socket::new_connection()
{
	return accept(_servfd, (struct sockaddr *)&_address, (socklen_t*)&_address);
}

/*------------------------------Other connection funcs------------------------------*/

struct pollfd	new_pollfd(int cli_sock)
{
	struct pollfd new_fd;
	int flags;

	new_fd.fd = cli_sock;
	new_fd.events = POLLIN;
	flags = fcntl(cli_sock, F_GETFL);
	fcntl(cli_sock, F_SETFL, flags | O_NONBLOCK);
	return new_fd;
}

t_client	accept_client(int fd)
{
	t_client new_client;

	new_client.fd = fd;
	new_client.request = Request();
	// beter als we hier een define gebruiken
	new_client.status = 200;
	std::cout << "Connected!" << std::endl;
	return new_client;
}

void	check_connection(t_data &data)
{
	for (int i = 0; i < data.socket_num; i++)
	{
		if (data.fds[i].revents & POLLIN)
		{
			int cli_sock;

			if ((cli_sock = data.sockets[i].new_connection()) < 0)
			{
				if (errno != EWOULDBLOCK)
					std::cout << "error occured" << std::endl;
			}
			else
			{
				data.fds.push_back(new_pollfd(cli_sock)); // error protection needed
				data.clients.push_back(accept_client(cli_sock));
			}
		}

	}
}
