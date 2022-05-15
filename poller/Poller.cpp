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

Poller::Poller(std::set<int> server_ports) : _cgi_socks(std::vector<CgiSocket*>()), _client_socks(std::vector<ClientSocket*>()) 
{
	for (std::set<int>::const_iterator it = server_ports.begin(); it != server_ports.end(); it++)
	{
		_serv_socks.push_back(new ServerSocket(AF_INET, SOCK_STREAM, 0, *it, INADDR_ANY, BACKLOG));
		_pollfds.push_back(addPoll(_serv_socks.back()->getFd()));
	}
}

pollfd			Poller::addPoll(int fd)
{
	pollfd newPoll;

	bzero(&newPoll, sizeof(newPoll));
	newPoll.fd = fd;
	newPoll.events = POLLIN | POLLOUT;
	return newPoll;
}

/*---------------------------------------------------------------*/
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

void			Poller::check_cli_socks()
{
	for (std::vector<ClientSocket*>::const_iterator it = _client_socks.begin(); it != _client_socks.end(); it++)
	{
		if (*it);
	}
}

/*---------------------------------------------------------------*/
void			Poller::deleteCgi(int index)
{
	int i = index - _serv_socks.size();

	_pollfds.erase(_pollfds.begin() + index);
	delete _cgi_socks[i];
	_cgi_socks.erase(_cgi_socks.begin() + i);
}

void			Poller::deleteCli(int index)
{
	int i = index - _serv_socks.size() - _cgi_socks.size();

	_pollfds.erase(_pollfds.begin() + index);
	delete _client_socks[i];
	_client_socks.erase(_client_socks.begin() + i);
}

/*---------------------------------------------------------------*/
CgiSocket		&Poller::get_cgi_from_index(int i)
{
	return *_cgi_socks[i - _serv_socks.size()];
}

ClientSocket	&Poller::get_cli_from_index(int i)
{
	return *_client_socks[i - _serv_socks.size() - _cgi_socks.size()];
}

/*---------------------------------------------------------------*/
pollfd			*Poller::preparePoll()
{

}

void			Poller::execute_poll(std::map<std::pair<int, std::string>, Server*>	table)
{
	std::cout << "Execute_poll: size: " << _pollfds.size() << std::endl;

	poll(&_pollfds[0], _pollfds.size(), -1);
	try
	{
		for (int i = _serv_socks.size(); i < _pollfds.size() - _serv_socks.size(); i++)
		{
			if (_pollfds[i].revents == 17) {
				deleteCgi(i);
				i--;
			}
			else if (_pollfds[i].revents & POLLIN)
				get_cgi_from_index(i).handle_pollin();
			else if (_pollfds[i].revents & POLLOUT)
				get_cgi_from_index(i).handle_pollout(table, *this);
		}
		for (int i = _serv_socks.size() + _cgi_socks.size() ; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 17) {
				deleteCli(i);
				i--;
			}
			else if (_pollfds[i].revents & POLLIN)
				get_cli_from_index(i).handle_pollin();
			else if (_pollfds[i].revents & POLLOUT)
				get_cli_from_index(i).handle_pollout(table, *this);
		}
		check_cli_socks();
		check_server_socks();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
