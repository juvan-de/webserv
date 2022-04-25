#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>

class Cgi
{
	private:
		/*--------------------------Member variables--------------------------*/

	public:
		/*----------------------------Coplien form----------------------------*/
		Cgi();
		Cgi(const Cgi &ref);
		Cgi& operator=(const Cgi &ref);
		~Cgi();

		/*--------------------------Member functions--------------------------*/

};

std::ostream&	operator<<(std::ostream &out, const Cgi &ref);

#endif
