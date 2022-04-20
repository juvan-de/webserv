#include "ClientSocket.hpp"
#include "output.hpp"

/*--------------------------------Coplien form--------------------------------*/
ClientSocket::ClientSocket()
{
	/*Constructor*/
	std::cout << YELLOW << "ClientSocket default constructor called" << RESET << std::endl;
}

ClientSocket::~ClientSocket()
{
	/*Destructor*/
	std::cout << YELLOW << "ClientSocket destructor called" << RESET << std::endl;
}

ClientSocket::ClientSocket(const ClientSocket &ref)
{
	/*Copy constructor*/
	std::cout << BOLD << "ClientSocket copy constructor called" << RESET << std::endl;
	*this = ref;
}

ClientSocket&	ClientSocket::operator=(const ClientSocket &ref)
{
	/*Assignation operator*/
	std::cout << BOLD << "ClientSocket assignation overload called" << RESET << std::endl;
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

std::ostream&	operator<<(std::ostream &out, const ClientSocket &ref)
{
	/*Output operator*/
	/*out << ClientSocket;
	return out;*/
}
