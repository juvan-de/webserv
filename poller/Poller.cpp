#include "Poller.hpp"

/*--------------------------------Coplien form--------------------------------*/
Poller::Poller()
{
	/*Constructor*/
}

Poller::~Poller()
{
	/*Destructor*/
}

Poller::Poller(const Poller &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Poller&	Poller::operator=(const Poller &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Poller::Poller(std::vector<int>	server_ports)
{
	for (std::vector<int>::const_iterator it = server_ports.begin(); it != server_ports.end(); it++)
		_serv_socks.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, *it, sizeof(sockaddr_in), 100));

}

void	Poller::add_cgi_sock(int fd)
{

}

void	Poller::execute_poll()
{

}
