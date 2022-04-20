#include "ServerSocket.hpp"
#include "output.hpp"

/*--------------------------------Coplien form--------------------------------*/
ServerSocket::ServerSocket()
{
	/*Constructor*/
	std::cout << YELLOW << "ServerSocket default constructor called" << RESET << std::endl;
}

ServerSocket::~ServerSocket()
{
	/*Destructor*/
	std::cout << YELLOW << "ServerSocket destructor called" << RESET << std::endl;
}

ServerSocket::ServerSocket(const ServerSocket &ref)
{
	/*Copy constructor*/
	std::cout << BOLD << "ServerSocket copy constructor called" << RESET << std::endl;
	*this = ref;
}

ServerSocket&	ServerSocket::operator=(const ServerSocket &ref)
{
	/*Assignation operator*/
	std::cout << BOLD << "ServerSocket assignation overload called" << RESET << std::endl;
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

std::ostream&	operator<<(std::ostream &out, const ServerSocket &ref)
{
	/*Output operator*/
	/*out << ServerSocket;
	return out;*/
}
