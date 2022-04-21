#ifndef TESTCLASS_HPP
# define TESTCLASS_HPP

# include <iostream>

class testclass
{
	private:
		/*--------------------------Member variables--------------------------*/

	public:
		/*----------------------------Coplien form----------------------------*/
		testclass();
		testclass(const testclass &ref);
		testclass& operator=(const testclass &ref);
		~testclass();

		/*--------------------------Member functions--------------------------*/

};

#endif
