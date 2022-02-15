#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
	private:
		/*--------------------------Member variables--------------------------*/

	public:
		/*----------------------------Coplien form----------------------------*/
		Client();
		Client(const Client &ref);
		Client& operator=(const Client &ref);
		~Client();

		/*--------------------------Member functions--------------------------*/

};

std::ostream&	operator<<(std::ostream &out, const Client &ref);

#endif
