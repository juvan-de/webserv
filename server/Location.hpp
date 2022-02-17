#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <set>
# include <map>
# include <deque>
# include <redir.hpp>

# define COLOR_WHITE_BOLD			"\033[0;37;01m"
# define COLOR_NORMAL_DIM			"\033[0;02m"
# define COLOR_NORMAL				"\033[0m"

class	Location
{
	private:
		std::string							_title;
		std::string							_root;
		unsigned long						_clientMaxBodySize;
		std::vector<std::string>			_index;
		bool								_autoindex;
		bool								_staticDir;
		std::map<std::string, std::string>	_cgi;
		std::set<std::string>				_limitExcept;
		std::string							_uploadStore;
		Redir								_redir;

	public:
		Location(std::string title = "");
		Location(std::deque<std::string>& file, std::string& title);
		Location(const Location& ref);
		Location&	operator= (const Location& ref);
		~Location();

	public:		/* -Setters- */
		void	setTitle(std::string& title);
		void	setRoot(std::vector<std::string>& line);
		void	setClientMaxBodySize(std::vector<std::string>& line);
		void	setIndex(std::vector<std::string>& line);
		void	setAutoindex(std::vector<std::string>& line);
		void	setStaticDir(std::vector<std::string>& line);
		void	addCgi(std::vector<std::string>& line);
		void	setLimitExcept(std::vector<std::string>& line);
		void	setUploadStore(std::vector<std::string>& line);
		void	setRedir(std::vector<std::string>& line);

	private:	/* -Setters Utils- */
		bool	isIn(const std::string& elem, const std::string array[], size_t size) const;
		void	_errorJumpTable(std::vector<std::string>& line);

	public:		/* -Getters- */
		const std::string&							getTitle() const;
		const std::string&							getRoot() const;
		unsigned long								getClientMaxBodySize() const;
		const std::vector<std::string>&				getIndex() const;
		bool										getAutoindex() const;
		bool										getStaticDir() const;
		const std::map<std::string, std::string>&	getCgi() const;
		const std::set<std::string>&				getLimitExcept() const;
		const std::string&							getUploadStore() const;
		const Redir&								getRedir() const;

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

		class MissingClosingBracket : public std::exception //done misschien shit verplaatsen
		{
			const char*	what (void) const throw()
			{
				std::string ret = COLOR_WHITE_BOLD;
				ret += "Missing closing Bracket for Locationblock"; 
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

		class cmbsUnitPrefix : public std::exception //done misschien shit verplaatsen
		{
			private:
				cmbsUnitPrefix();
			protected:
				std::vector<std::string>	_line;
				std::string					_unitPrefix;

			public:
				cmbsUnitPrefix(std::vector<std::string>& line) : _line(line)
				{
					size_t idx = line[1].find_first_not_of("0123456789");
					this->_unitPrefix = std::string(line[1], idx);
				}
				virtual ~cmbsUnitPrefix() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Invalid unit prefix\n";
					ret += COLOR_NORMAL;
					ret += "found: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'";
					ret += _unitPrefix; 
					ret += "'";
					ret += COLOR_NORMAL;
					ret += " expected: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'k'/'m'/'g'\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};

		class aiElemNotRecognized : public std::exception //done misschien shit verplaatsen
		{
			private:
				aiElemNotRecognized();
			protected:
				std::vector<std::string>	_line;
				std::string					_elem;

			public:
				aiElemNotRecognized(std::vector<std::string>& line) : _line(line), _elem(line[1]) {}
				virtual ~aiElemNotRecognized() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Autoindex element not recognized\n";
					ret += COLOR_NORMAL;
					ret += "found: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'";
					ret += _elem; 
					ret += "'";
					ret += COLOR_NORMAL;
					ret += " expected: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'on'/'off'\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};

		class sdElemNotRecognized : public std::exception //done misschien shit verplaatsen
		{
			private:
				sdElemNotRecognized();
			protected:
				std::vector<std::string>	_line;
				std::string					_elem;

			public:
				sdElemNotRecognized(std::vector<std::string>& line) : _line(line), _elem(line[1]) {}
				virtual ~sdElemNotRecognized() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Static_dir element not recognized\n";
					ret += COLOR_NORMAL;
					ret += "found: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'";
					ret += _elem; 
					ret += "'";
					ret += COLOR_NORMAL;
					ret += " expected: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'true'/'falses'\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};
	
		class leInvalidMethod : public std::exception //done misschien shit verplaatsen
		{
			private:
				leInvalidMethod();
			protected:
				std::vector<std::string>	_line;
				std::string					_elem;

			public:
				leInvalidMethod(std::vector<std::string>& line, std::string elem) : _line(line), _elem(elem) {}
				virtual ~leInvalidMethod() throw() {}
				const char*	what (void) const throw()
				{
					std::string ret;
					ret += COLOR_WHITE_BOLD;
					ret += "Invalid HTTP method\n";
					ret += COLOR_NORMAL;
					ret += "found: ";
					ret += COLOR_WHITE_BOLD;
					ret += "'" + this->_elem + "'\n";
					ret += COLOR_NORMAL_DIM;
					ret += "line:";
					for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
						ret += " " + *it;
					return ret.c_str();
				}
		};
};//end Location class

std::ostream&	operator<< (std::ostream& out, const Location& obj);

#endif
