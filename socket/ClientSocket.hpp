#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <Socket.hpp>

class ClientSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int		status;
		Request	request;

	public:
		/*--------------------------Member functions--------------------------*/

};

#endif
