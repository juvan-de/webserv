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

Poller::Poller(std::set<int> server_ports) : _cgi_socks(std::vector<pollfd>()), _client_socks(std::vector<ClientSocket*>()) 
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

t_poll			*Poller::newSocket(CgiSocket *cgi)
{
	t_poll *poll = new t_poll;

	poll->cgi = cgi;
	poll->cli = NULL;

	return poll;
}

t_poll			*Poller::newSocket(ClientSocket *cli)
{
	t_poll *poll = new t_poll;

	poll->cli = cli;
	poll->cgi = NULL;

	return poll;
}

void			Poller::deleteSock(int index)
{
	int cli_i = index - _serv_socks.size();

	_pollfds.erase(_pollfds.begin() + index);
	delete _client_socks[cli_i];
	_client_socks.erase(_client_socks.begin() + cli_i);
}

void			Poller::check_server_socks()
{
	for (int i = 0 ; i < _serv_socks.size(); i++)
	{
		try
		{
			if (_pollfds[i].revents & POLLIN)
			{
				_sockets.push_back(newSocket(_serv_socks[i]->get_new_cli()));
				// _client_socks.push_back(_serv_socks[i]->get_new_cli());
				_pollfds.push_back(addPoll(_sockets.back()->cli->getFd()));
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}


ClientSocket	&Poller::get_cli_from_index(int i)
{
	return *_client_socks[i - _serv_socks.size()];
}

pollfd			*Poller::preparePoll()
{
	std::vector<pollfd> pollfds;


}

void			Poller::execute_poll(std::map<std::pair<int, std::string>, Server*>	table)
{
	std::cout << "Execute_poll: size: " << _pollfds.size() << std::endl;
	poll(&_pollfds[0], _pollfds.size(), -1);
	try
	{
		for (int i = _serv_socks.size(); i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 17) {
				deleteSock(i);
				i--;
			}
			else if (_pollfds[i].revents & POLLIN)
				getSock(i).handle_pollin();
			else if (_pollfds[i].revents & POLLOUT)
				getSock(i).handle_pollout(table, *this);
		}
		check_server_socks();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
