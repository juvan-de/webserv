#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <set>

# include "Location.hpp"
# include "webservException.hpp"

class	Server
{
	private:
		std::set<int>					_listen;
		std::map<std::string, Location>	_locations;
		std::map<int, std::string>		_errorPage;
		std::set<std::string>			_serverName;

		void	_checkVarSet();
		void	_errorJumpTable(std::vector<std::string>& line);

	public:
		Server ();
		Server (const Server& ref);
		Server (std::deque<std::string>& file, const std::string& curWorkDir);
		Server&	operator= (const Server& ref);
		~Server ();

		/* -Setters- */
		void	setListen(std::vector<std::string>& line);
		void	addLocation(std::deque<std::string>& file, std::string& title, const std::string& curWorkDir);
		void	addErrorPage(std::vector<std::string>& line);
		void	setServerName(std::vector<std::string>& line);

		/* -Getters- */
		const std::set<int>&							getListen() const;
		const std::map<std::string, Location>&			getLocations() const;
		Location&										getLocation(const std::string& key);
		const std::map<int, std::string>&				getErrorPages() const;
		const std::string&								getErrorPage() const;
		const std::set<std::string>&					getServerName() const;
		std::map<std::string, Location>::const_iterator	getRightLocation(const std::string& request_loc) const;

		/* Not parse exception */
		class LocationDoesNotExist : public std::exception
		{
			const char*	what(void) const throw()
			{
				return ("This Location does not exist");
			};
		};
};

std::ostream&	operator<< (std::ostream& out, const Server& obj);

#endif
