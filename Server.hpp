#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include <map>

# include "Location.hpp"

class	Server
{
	private:
		std::map<std::string, Location>	location;
		std::map<int, std::string>		error_page;
		std::set<std::string>			server_name;

	public:
		Server ();
		Server (const Server& ref);
		Server&	operator= (const Server& ref);
		~Server ();

};

#endif
