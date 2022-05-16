#include "Poller.hpp"

/*--------------------------------Coplien form--------------------------------*/
Poller::Poller()
{
	/*Constructor*/
}

Poller::~Poller()
{
	/*Destructor*/
	// for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	// 	deleteSocket(it->fd);
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
		this->_lookup = ref._lookup;
		this->_pollfds = ref._pollfds;
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

static pollfd	addPoll(int fd)
{
	pollfd newPoll;

	bzero(&newPoll, sizeof(newPoll));
	newPoll.fd = fd;
	newPoll.events = POLLIN | POLLOUT;
	return newPoll;
}
/*------------------------------private functions-----------------------------*/

void			Poller::addSocket(ServerSocket *serv)
{
	int fd = serv->getFd();
	_lookup.insert(std::pair<int, t_type>(fd, SERV));
	_serv_socks.insert(std::pair<int, ServerSocket*>(fd, serv));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::addSocket(ClientSocket *cli)
{
	int fd = cli->getFd();
	_lookup.insert(std::pair<int, t_type>(fd, CLI));
	_client_socks.insert(std::pair<int, ClientSocket*>(fd, cli));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::addSocket(CgiSocket *cgi)
{
	int fd = cgi->getFd();
	_lookup.insert(std::pair<int, t_type>(fd, CGI));
	_cgi_socks.insert(std::pair<int, CgiSocket*>(fd, cgi));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::deleteSocket(int fd)
{
	t_type type = _lookup.find(fd)->second;

	switch (type)
	{
	case SERV:
		delete _serv_socks.find(fd)->second;
		_serv_socks.erase(_serv_socks.find(fd));
		break;
	case CLI:
		delete _client_socks.find(fd)->second;
		_client_socks.erase(_client_socks.find(fd));
		break;
	case CGI:
		delete _cgi_socks.find(fd)->second;
		_cgi_socks.erase(_cgi_socks.find(fd));
		break;
	}
	for (std::vector<pollfd>::const_iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd == fd)
		{
			_pollfds.erase(it);
			break ;
		}
	}
}

void			Poller::handleServ(std::vector< std::pair<int, short> > servers)
{
	for (std::vector< std::pair<int, short> >::const_iterator it = servers.begin(); it != servers.end(); it++)
		if (it->second & POLLIN)
			addSocket(_serv_socks.find(it->first)->second->get_new_cli());
}

void			Poller::handleCli(std::vector< std::pair<int, short> > clients, std::map<std::pair<int, std::string>, Server*> table)
{
	for (std::vector< std::pair<int, short> >::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		switch (it->second)
		{
		case 17:
			deleteSocket(it->first);
			break;
		case (POLLSTANDARD & (POLLIN | POLLOUT)):
			_client_socks.find(it->first)->second->handle_pollin();
			// if (_client_socks.find(it->first)->second->hasCgi())
			// 	addSocket(_client_socks.find(it->first)->second->getCgi());
			break;
		case (POLLSTANDARD & POLLOUT):
			_client_socks.find(it->first)->second->handle_pollout(table, *this);
			break;
		}
	}
}

void			Poller::handleCgi(std::vector< std::pair<int, short> > cgi)
{
	for (std::vector< std::pair<int, short> >::const_iterator it = cgi.begin(); it != cgi.end(); it++)
	{
		switch (it->second)
		{
		case (17):
			deleteSocket(it->first);
			break;
		case (POLLSTANDARD & POLLIN):
			// _cgi_socks.find(it->first)->second->handle_pollin();
			break;
		case (POLLSTANDARD & POLLOUT):
			// _cgi_socks.find(it->first)->second->handle_pollout();
			break;
		}
	}
}
/*-------------------------------public functions-----------------------------*/

Poller::Poller(std::set<int> server_ports)
{
	for (std::set<int>::const_iterator it = server_ports.begin(); it != server_ports.end(); it++)
		addSocket(new ServerSocket(AF_INET, SOCK_STREAM, 0, *it, INADDR_ANY, BACKLOG));
}

static void		getActiveFd(std::vector< std::pair<int, short> > &vec, pollfd poll)
{
	switch (poll.revents)
	{
	case 17:
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
		break;
	case (POLLSTANDARD & (POLLIN | POLLOUT)):
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
		break;
	case (POLLSTANDARD & POLLIN):
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
		break;
	case (POLLSTANDARD & POLLOUT):
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
		break;
	}
}

void			Poller::executePoll(std::map<std::pair<int, std::string>, Server*> table)
{
	std::vector< std::pair<int, short> > servers;
	std::vector< std::pair<int, short> > clients;
	std::vector< std::pair<int, short> > cgi;

	std::cout << "ExecutePoll: size: " << _pollfds.size() << std::endl;
	poll(&_pollfds[0], _pollfds.size(), -1);
	try
	{
		for (std::vector<pollfd>::const_iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		{
			// std::cout << "Poll loop fd: " << it->fd << ", revents: " << it->revents << std::endl;
			t_type type = _lookup.find(it->fd)->second;

			switch (type)
			{
			case SERV:
				getActiveFd(servers, *it);
				break;
			case CLI:
				getActiveFd(clients, *it);
				break;
			case CGI:
				getActiveFd(cgi, *it);
				break;
			}
		}
		std::cout << "ExecutePoll serves size: " << servers.size() << std::endl;
		std::cout << "ExecutePoll clients size: " << clients.size() << std::endl;
		std::cout << "ExecutePoll cgi size: " << cgi.size() << std::endl;
		// handleCgi(cgi);
		handleCli(clients, table);
		handleServ(servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
