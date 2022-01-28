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
		std::string							_root;
		int									_clientMaxBodySize;
		std::vector<std::string				_index;
		bool								_autoindex;
		bool								_static_dir;
		std::map<std::string, std::string>	_cgi;
		std::set<std::string>				_limitExcept;
		std::string							_uploadStore;
		Redir								_redir;

	public:
		Location();
		{
			while
				this->_redir = Redir();
		}
		Location(const Location& ref);
		Location&	operator= (const Location& ref);
		~Location();

		// setters
		void	setRoot();
		void	setClientMaxBodySize();
		void	setIndex();
		void	setAutoindex();
		void	setStaticDir();
		void	addCgi();
		void	setLimitExcept();
		void	setUploadStore();
		void	setRedir();
}

#endif
