#include "CgiSocket.hpp"

/*--------------------------------Coplien form--------------------------------*/
CgiSocket::CgiSocket()
{
	/*Constructor*/
}

CgiSocket::~CgiSocket()
{
	/*Destructor*/
}

CgiSocket::CgiSocket(const CgiSocket &ref)
{
	/*Copy constructor*/
	*this = ref;
}

CgiSocket&	CgiSocket::operator=(const CgiSocket &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/
