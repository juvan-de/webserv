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
		_serv_socks.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, *it, INADDR_ANY, BACKLOG));
		_pollfds.push_back(addPoll(_serv_socks.back()->getFd()));
	}
}

pollfd			Poller::addPoll(int fd)
{
	pollfd newPoll;

	// Set poll
	bzero(&newPoll, sizeof(newPoll));
	newPoll.fd = fd;
	newPoll.events = POLLIN | POLLOUT;
	return newPoll;
}

void			Poller::add_cgi_sock(int fd)
{
	_cgi_socks.push_back(addPoll(fd));
}

void			Poller::check_server_socks()
{
	for (int i = 0 ; i < _serv_socks.size(); i++)
	{
		try
		{
			if (_pollfds[i].revents & POLLIN)
			{
				_client_socks.push_back(_serv_socks[i]->get_new_cli());
				_pollfds.push_back(addPoll(_client_socks.back()->getFd()));
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

void			Poller::deleteCli(int index)
{
	int cli_i = index - _serv_socks.size();

	_pollfds.erase(_pollfds.begin() + index);
	delete _client_socks[cli_i];
	_client_socks.erase(_client_socks.begin() + cli_i);
}

ClientSocket	&Poller::get_cli_from_index(int i)
{
	return *_client_socks[i - _serv_socks.size()];
}

void			Poller::execute_poll(std::map<std::pair<int, std::string>, Server*>	table)
{
	std::cout << "Execute_poll: size: " << _pollfds.size() << std::endl;

	poll(&_pollfds[0], _pollfds.size(), -1);
	try
	{
		for (int i = _serv_socks.size(); i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 17)
				deleteCli(i);
			else if (_pollfds[i].revents & POLLIN)
				get_cli_from_index(i).handle_pollin();
			else if (_pollfds[i].revents & POLLOUT)
				get_cli_from_index(i).handle_pollout(table);
		}
		check_server_socks();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
