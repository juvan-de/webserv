#ifndef BADINIT_HPP
# define BADINIT_HPP

# include <exception>

class BadInit : public std::exception
{
	public:
		/*----------------------------Coplien form----------------------------*/
		BadInit() {};
		BadInit(const BadInit &ref) { *this = ref; };
		BadInit& operator=(const BadInit &ref) { if (this != &ref) {} return *this; };
		~BadInit() throw() {};
		/*--------------------------Exception Classes-------------------------*/
		virtual const char* what() const throw()
		{
			return "Error: could not prepare port";
		}
		/*--------------------------Member functions--------------------------*/

};
#endif
