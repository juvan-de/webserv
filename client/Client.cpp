#include "Client.hpp"
#include "output.hpp"

/*--------------------------------Coplien form--------------------------------*/
Client::Client()
{
	/*Constructor*/
	std::cout << YELLOW << "Client default constructor called" << RESET << std::endl;
}

Client::~Client()
{
	/*Destructor*/
	std::cout << YELLOW << "Client destructor called" << RESET << std::endl;
}

Client::Client(const Client &ref)
{
	/*Copy constructor*/
	std::cout << BOLD << "Client copy constructor called" << RESET << std::endl;
	*this = ref;
}

Client&	Client::operator=(const Client &ref)
{
	/*Assignation operator*/
	std::cout << BOLD << "Client assignation overload called" << RESET << std::endl;
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

std::ostream&	operator<<(std::ostream &out, const Client &ref)
{
	/*Output operator*/
	/*out << Client;
	return out;*/
}
