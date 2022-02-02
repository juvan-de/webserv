#ifndef REDIR_HPP
# define REDIR_HPP

#include <iostream>

class Redir
{
	private:
		unsigned int	_statusCode;
		std::string		_location;

	public:
		Redir(std::string statuscode = "", std::string location = "");
		Redir&	operator= (const Redir& ref);
		Redir(const Redir& ref);
		~Redir();

		void			setStatusCode (std::string);
		void			setStatusCode (unsigned int);
		void			setLocation (std::string);

		unsigned int	getStatusCode () const;
		std::string		getLocation () const;
};

std::ostream& operator<< (std::ostream& out, const Redir& obj);

#endif
