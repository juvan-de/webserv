#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <set>
# include <map>
# include <deque>
# include "redir.hpp"

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

		/* -Setters- */
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

		/* -Getters- */
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
		class ArgumentIncorrect : public std::exception
		{
			// private:
			// 	std::vector<std::string>	_line;
			
			virtual const char*	what(void) const throw()
			{
				// std::string	ret;

				// ret = "Incorrect argument amount:";
				// for (size_t i = 0; i < this->_line.size(); i++)
				// 	ret += " " + this->_line[i];
				// return ret.c_str();
				return "Incorrect argument amount";
			}

			// ArgumentIncorrect(std::vector<std::string> line) : _line(line)
			// {
			// 	return ;
			// }
		};

		class ClosingLocation : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Location block is not closed. Use '}'.";
			}
		};

		class ElemNotRecognized : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Location elem is not recognized";
			}
		};

		class CMBSToManyLetters : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "client_max_body_size can have one adjacent letter. This letter can be [kmg].";
			}
		};

		class CMBSLetterNotRecognized : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "client_max_body_size letter not recognized. Last char can be [kmg].";
			}
		};

		class AIElemNotRecognized : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Wrong argument. Autoindex can be [\"on\", \"off\"].";
			}
		};

		class SDElemNotRecognized : public std::exception
		{
			const char*	what (void) const throw()
			{
				return "Wrong argument. static_dir can be [\"true\", \"false\"].";
			}
		};
};//end Location class

std::ostream&	operator<< (std::ostream& out, const Location& obj);

#endif
