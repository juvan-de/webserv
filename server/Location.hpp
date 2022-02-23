#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <set>
# include <map>
# include <deque>
# include <redir.hpp>

# include "webservException.hpp"
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
};//end Location class

std::ostream&	operator<< (std::ostream& out, const Location& obj);

#endif
