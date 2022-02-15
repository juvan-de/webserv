#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <set>

# include "Location.hpp"

class	Server
{
	private:
		std::set<int>					_listen;
		std::map<std::string, Location>	_locations;
		std::map<int, std::string>		_errorPage;
		std::set<std::string>			_serverName;

	public:
		Server ();
		Server (const Server& ref);
		Server (std::deque<std::string>& file);
		Server&	operator= (const Server& ref);
		~Server ();

		/* -Setters- */
		void	setListen(std::vector<std::string>& line);
		void	addLocation(std::deque<std::string>& file, std::string& title);
		void	addErrorPage(std::vector<std::string>& line);
		void	setServerName(std::vector<std::string>& line);
		/* -Getters- */
		const std::set<int>&					getListen() const;
		const std::map<std::string, Location>&	getLocations() const;
		Location&								getLocation(const std::string& key);
		const std::map<int, std::string>&		getErrorPage() const;
		const std::set<std::string>&			getServerName() const;

		/* -Exception- */
		class ArgumentIncorrect : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Incorrect argument amount";
			}
		};

		class ElemNotRecognized : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Location elem is not recognized";
			}
		};

		class NotANumber : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Element is not a number";
			}
		};
};

std::ostream&	operator<< (std::ostream& out, const Server& obj);

#endif
