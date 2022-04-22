#include <Poller.hpp>

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
		this->_cgi_socks = ref._cgi_socks;
		this->_serv_socks = ref._serv_socks;
		this->_client_socks = ref._client_socks;
		this->_pollfds = ref._pollfds;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Poller::Poller(std::vector<int>	server_ports) : _cgi_socks(std::vector<pollfd>()), _client_socks(std::vector<ClientSocket*>()) 
{
	for (std::vector<int>::const_iterator it = server_ports.begin(); it != server_ports.end(); it++)
	{
		_serv_socks.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, *it, sizeof(sockaddr_in), 100));
		_pollfds.push_back(_serv_socks.back()->getPoll());
	}
}

void	Poller::add_cgi_sock(int fd)
{
	pollfd	newpoll;
	
	newpoll.fd = fd;
	newpoll.revents = POLLIN | POLLOUT;
	_cgi_socks.push_back(newpoll);
}

void	Poller::execute_poll()
{
	std::vector<pollfd> pollfds;
	pollfds.reserve(_serv_socks.size() + _serv_socks.size() + _client_socks.size());
	
}
