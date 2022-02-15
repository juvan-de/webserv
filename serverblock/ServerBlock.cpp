#include "ServerBlock.hpp"

/*--------------------------------Coplien form--------------------------------*/
ServerBlock::ServerBlock()
{
	/*Constructor*/
}

ServerBlock::~ServerBlock()
{
	/*Destructor*/
}

ServerBlock::ServerBlock(const ServerBlock &ref)
{
	/*Copy constructor*/
	*this = ref;
}

ServerBlock&	ServerBlock::operator=(const ServerBlock &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/

std::ostream&	operator<<(std::ostream &out, const ServerBlock &ref)
{
	/*Output operator*/
	/*out << ServerBlock;
	return out;*/
}