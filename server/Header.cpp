#include "Header.hpp"


Header::Header(std::string request)
{
	size_t size = request.find(' ');
	switch(size)
	{
		case 3 : this->_type = GET; break;
		case 4 : this->_type = POST; break;
		case 6 : this->_type = DELETE; break;
	}
	while (request)
	{
		
	}
}

Header::Header(const Header& ref)
{
	*this = ref;
}

Header&	Header::operator=(const Header& ref)
{

}

Header::~Header()
{

}
