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
		std::string							root;
		int									client_max_body_size;
		std::vector<std::string				index;
		bool								autoindex;
		bool								static_dir;
		std::map<std::string, std::string>	cgi;
		std::set<std::string>				limit_except;
		std::string							upload_store;
		t_redir								redir;

	public:
		Location();
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
