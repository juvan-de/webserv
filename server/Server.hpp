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
		class ArgumentIncorrect : public std::exception //done misschien shit verplaatsen
		{
			private:
				ArgumentIncorrect();
			protected:
				std::vector<std::string>	_line;
			public:
				ArgumentIncorrect(std::vector<std::string>& line) : _line(line) {}
				virtual ~ArgumentIncorrect() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Invalid argument amount\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};

		class ElemNotRecognized : public std::exception //done misschien shit verplaatsen
		{
			private:
				ElemNotRecognized();
			protected:
				std::vector<std::string>	_line;
			public:
			ElemNotRecognized(std::vector<std::string>& line) : _line(line) {}
			virtual ~ElemNotRecognized() throw() {}
			const char*	what (void) const throw()
			{
				std::string ret;
				ret += COLOR_WHITE_BOLD;
				ret += "Element no recognized\n";
				ret += COLOR_NORMAL;
				ret += "found: ";
				ret += COLOR_WHITE_BOLD;
				ret += "'";
				ret += _line[0]; 
				ret += "'\n";
				ret += COLOR_NORMAL_DIM;
				ret += "line:";
				for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
					ret += " " + *it;
				return ret.c_str();
			}

		};

		class epNotANumber : public std::exception
		{
			private:
				epNotANumber();
			protected:
				std::vector<std::string>	_line;
			public:
				epNotANumber(std::vector<std::string>& line) : _line(line) {}
				virtual ~epNotANumber() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Element is not a number\n";
					ret += "found: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'";
					ret += _line[1]; 
					ret += "'\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};
};

std::ostream&	operator<< (std::ostream& out, const Server& obj);

#endif
