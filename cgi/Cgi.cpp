#include "Cgi.hpp"

/*--------------------------------Coplien form--------------------------------*/
Cgi::Cgi()
{
	/*Constructor*/
}

Cgi::~Cgi()
{
	/*Destructor*/
}

Cgi::Cgi(const Cgi &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Cgi&	Cgi::operator=(const Cgi &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

Cgi(std::string path, map<std::string, std::string> arguments) : _cgiIn(-1), _cgiOut(-1), _cgiPID(-1)
{
	int	inputFD[2];
	int	outputFD[2];
}

