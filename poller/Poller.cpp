#include "Poller.hpp"
#include <unistd.h> // sleep

/*--------------------------------Coplien form--------------------------------*/
Poller::~Poller()
{
	/*Destructor*/
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		deleteSocket(it->fd);
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

void			Poller::changePoll(int oldFd, int newFd)
{
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
	{
		if (it->fd == oldFd)
		{
			std::map<int, CgiSocket*>::iterator cgi_it = _cgi_socks.find(it->fd);
			CgiSocket *socket = cgi_it->second;

			_cgi_socks.erase(cgi_it);
			_cgi_socks.insert(std::pair<int, CgiSocket*>(newFd, socket));
			_lookup.erase(_lookup.find(oldFd));
			_lookup.insert(std::pair<int, t_type>(newFd, T_CGI));
			it->fd = newFd;
			return ;
		}
	}
}

void			Poller::addSocket(ServerSocket *serv)
{
	int fd = serv->getFd();
	_lookup.insert(std::pair<int, t_type>(fd, T_SERV));
	_serv_socks.insert(std::pair<int, ServerSocket*>(fd, serv));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::addSocket(ClientSocket *cli)
{
	int fd = cli->getFd();
	_lookup.insert(std::pair<int, t_type>(fd, T_CLI));
	_client_socks.insert(std::pair<int, ClientSocket*>(fd, cli));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::addSocket(CgiSocket *cgi)
{
	int fd;
	
	if (cgi->getBodyStatus() == true)
		fd = cgi->getFdIn();
	else
		fd = cgi->getFdOut();
	_lookup.insert(std::pair<int, t_type>(fd, T_CGI));
	_cgi_socks.insert(std::pair<int, CgiSocket*>(fd, cgi));
	_pollfds.push_back(addPoll(fd));
}

void			Poller::deleteSocket(int fd)
{
	t_type type = _lookup.find(fd)->second;

	switch (type)
	{
	case T_SERV:
		delete _serv_socks.find(fd)->second;
		_serv_socks.erase(_serv_socks.find(fd));
		break;
	case T_CLI:
		delete _client_socks.find(fd)->second;
		_client_socks.erase(_client_socks.find(fd));
		break;
	case T_CGI:
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
	_lookup.erase(_lookup.find(fd));
}

void			Poller::handleServ(std::vector< std::pair<int, short> > servers)
{
	usleep(500);
	for (std::vector< std::pair<int, short> >::const_iterator it = servers.begin(); it != servers.end(); it++)
		if (it->second & POLLIN)
			addSocket(_serv_socks.find(it->first)->second->get_new_cli());
}

void			Poller::handleCli(std::vector< std::pair<int, short> > clients, std::map<std::pair<int, std::string>, Server*> table)
{
	for (std::vector< std::pair<int, short> >::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		ClientSocket *socket = _client_socks.find(it->first)->second;

		if (it->second & POLLHUP)
			deleteSocket(it->first);
		else if (it->second & POLLIN)
		{
			socket->handle_pollin(table);
			if (socket->getCgi() && socket->getCgi()->getStatus() == CREATED)
			{
				addSocket(socket->getCgi());
				socket->getCgi()->setSatus(ADDED);
			}
		}
		else if (it->second & POLLOUT)
		{
			if (socket->getRequest().getType() == NOTSET)
				deleteSocket(it->first);
			else if (socket->getRequest().readyForParse())
				socket->handle_pollout();
		}
	}
}

void			Poller::handleCgi(std::vector< std::pair<int, short> > cgi)
{
	for (std::vector< std::pair<int, short> >::const_iterator it = cgi.begin(); it != cgi.end(); it++)
	{
		CgiSocket *socket = _cgi_socks.find(it->first)->second;

		if (it->second & POLLHUP || socket->getStatus() == FINISHED)
			deleteSocket(it->first);
		else if (it->second & POLLIN)
			socket->read_from_cgi();
		else if (it->second & POLLOUT)
		{
			if (socket->getBodyStatus() == true)
			{
				usleep(500);
				socket->write_to_cgi();
				changePoll(socket->getFdIn(), socket->getFdOut());
			}
			else
				_cgi_socks.find(it->first)->second->setSatus(SENT);
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
	if (poll.revents & POLLHUP)
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
	else if (poll.revents & POLLIN)
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
	else if (poll.revents & POLLOUT)
		vec.push_back(std::pair<int, short>(poll.fd, poll.revents));
}

void			Poller::executePoll(std::map<std::pair<int, std::string>, Server*> table)
{
	std::vector< std::pair<int, short> > servers;
	std::vector< std::pair<int, short> > clients;
	std::vector< std::pair<int, short> > cgi;

	// std::cout << "pollfd size: " << _pollfds.size() << std::endl;
	// std::cout << "cli size: " << _client_socks.size() << std::endl;
	poll(_pollfds.data(), _pollfds.size(), -1);
	try
	{
		for (std::vector<pollfd>::const_iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		{
			t_type type = _lookup.find(it->fd)->second;

			switch (type)
			{
			case T_SERV:
				getActiveFd(servers, *it);
				break;
			case T_CLI:
				getActiveFd(clients, *it);
				break;
			case T_CGI:
				getActiveFd(cgi, *it);
				break;
			}
		}
		// std::cout << "active fds, server: " << servers.size() << ", cli: " << clients.size() << ", cgi: " << cgi.size() << std::endl;
		handleCgi(cgi);
		handleCli(clients, table);
		handleServ(servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
