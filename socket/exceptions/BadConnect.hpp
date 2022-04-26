#ifndef BADCONNECT_HPP
# define BADCONNECT_HPP

# include <exception>

class BadConnect : public std::exception
{
	public:
		/*----------------------------Coplien form----------------------------*/
		BadConnect() {};
		BadConnect(const BadConnect &ref) { *this = ref; };
		BadConnect& operator=(const BadConnect &ref) { if (this != &ref) {} return *this; };
		~BadConnect() throw() {};
		/*--------------------------Exception Classes-------------------------*/
		virtual const char* what() const throw()
		{
			return "Error: could not accept client";
		}
		/*--------------------------Member functions--------------------------*/

};
#endif