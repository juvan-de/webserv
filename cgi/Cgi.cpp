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

std::ostream&	operator<<(std::ostream &out, const Cgi &ref)
{
	/*Output operator*/
	/*out << Cgi;
	return out;*/
}