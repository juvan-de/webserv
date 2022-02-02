#ifndef LOCATION_HPP
# define LOCATION_HPP

typedef struct		s_redir
{
	int				status_code;
	std::string		location;
}					t_redir;

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
		Location();
		Location(const Location& ref);
		Location&	operator= (const Location& ref);
		~Location();

		// setters
		// void	setTitle();
		void	setRoot(std::vector<std::string>& line);
		void	setClientMaxBodySize(std::vector<std::string>& line);
		void	setIndex(std::vector<std::string>& line);
		void	setAutoindex(std::vector<std::string>& line);
		void	setStaticDir(std::vector<std::string>& line);
		void	addCgi(std::vector<std::string>& line);
		void	setLimitExcept(std::vector<std::string>& line);
		void	setUploadStore(std::vector<std::string>& line);
		void	setRedir(std::vector<std::string>& line);

		//getters
		const std::string&							getRoot() const;
		const unsigned long							getClientMaxBodySize() const;
		const std::vector<std::string>&				getIndex() const;
		const bool									getAutoindex() const;
		const bool									getStaticDir() const;
		const std::map<std::string, std::string>&	getCgi() const;
		const std::set<std::string>&				getLimitExcept() const;
		const std::string&							getUploadStore() const;
		const Redir&								getRedir() const;

		class ArgumentIncorrect : public std::exception
		{
			private:
				std::vector<std::string>	_line;
			
			const char*	what(void) const throw()
			{
				std::string	ret;

				ret = "Incorrect argument amount:"
				for (size_t i = 0; i < this->_line.size(); i++)
					ret += " " + this->_line[i];
				return ret.c_str();
			};

			ArgumentIncorrect(std::vector<std::string> line) : _line(line);
		};

#endif
