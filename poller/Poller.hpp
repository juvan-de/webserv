#ifndef POLLER_HPP
# define POLLER_HPP

# include <iostream>

class Poller
{
	private:
		/*--------------------------Member variables--------------------------*/
		// vec clients socks
		// vec serv socks
		// vec socket

	public:
		/*----------------------------Coplien form----------------------------*/
		Poller();
		Poller(const Poller &ref);
		Poller& operator=(const Poller &ref);
		~Poller();

		/*--------------------------Member functions--------------------------*/
		accept_new_clients
};

#endif
