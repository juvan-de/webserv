#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>

class ClientSocket
{
	private:
		/*--------------------------Member variables--------------------------*/

	public:
		/*----------------------------Coplien form----------------------------*/
		ClientSocket();
		ClientSocket(const ClientSocket &ref);
		ClientSocket& operator=(const ClientSocket &ref);
		~ClientSocket();

		/*--------------------------Member functions--------------------------*/

};

std::ostream&	operator<<(std::ostream &out, const ClientSocket &ref);

#endif
