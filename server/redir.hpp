#ifndef REDIR_HPP
# define REDIR_HPP

#include <iostream>
# include <webservException.hpp>

class Redir
{
	private:
		unsigned int	_statusCode;
		std::string		_location;
		bool			_isSet;

	public:
		Redir(std::vector<std::string>& line, const std::string statuscode = "", const std::string location = "");
		Redir&	operator= (const Redir& ref);
		Redir(const Redir& ref);
		Redir();
		~Redir();

		void			setStatusCode (const std::string& statusCode, const std::vector<std::string>& line);
		void			setStatusCode (unsigned int);
		void			setLocation (std::string);

		unsigned int	getStatusCode () const;
		std::string		getLocation () const;
		bool			isSet() const;
};

std::ostream& operator<< (std::ostream& out, const Redir& obj);

#endif
