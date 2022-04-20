#include "Poller.hpp"

/*--------------------------------Coplien form--------------------------------*/
Poller::Poller()
{
	/*Constructor*/
}

Poller::~Poller()
{
	/*Destructor*/
}

Poller::Poller(const Poller &ref)
{
	/*Copy constructor*/
	*this = ref;
}

Poller&	Poller::operator=(const Poller &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/
