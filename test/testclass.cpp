#include "testclass.hpp"

/*--------------------------------Coplien form--------------------------------*/
testclass::testclass()
{
	/*Constructor*/
	std::cout << "construct" << std::endl;
}

testclass::~testclass()
{
	/*Destructor*/
		std::cout << "destruct" << std::endl;

}

testclass::testclass(const testclass &ref)
{
	/*Copy constructor*/
	*this = ref;
}

testclass&	testclass::operator=(const testclass &ref)
{
	/*Assignation operator*/
	if (this != &ref)
	{
		/* assign member variables*/
	}
	return *this;
}
/*--------------------------------Coplien form--------------------------------*/
