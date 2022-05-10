#ifndef WEBSERV_EXCEPTIONS_HPP
# define WEBSERV_EXCEPTIONS_HPP

# include <iostream>
# include <vector>
# include <string>

# define COLOR_WHITE_BOLD			"\033[0;37;01m"
# define COLOR_NORMAL_DIM			"\033[0;02m"
# define COLOR_NORMAL				"\033[0m"

class WrongFile : public std::exception
{
	private:
		WrongFile();
	protected:
		const std::string	_filename;
		std::string			_ret;
	public:
		WrongFile(const std::string filenames);
		virtual ~WrongFile() throw();
		const char*	what (void) const throw();
};

class ArgumentIncorrect : public std::exception
{
	private:
		ArgumentIncorrect();
	protected:
		const std::vector<std::string>	_line;
		std::string			_ret;
	public:
		ArgumentIncorrect(const std::vector<std::string>& line);
		virtual ~ArgumentIncorrect() throw();
		const char*	what (void) const throw();
};

class MissingClosingBracket : public std::exception
{
	private:
		MissingClosingBracket();
	protected:
		const std::string	_location;
		std::string			_ret;
	public:
		MissingClosingBracket(const std::string& loc);
		virtual ~MissingClosingBracket() throw();
		const char*	what (void) const throw();
};

class DirectiveNotRecognized : public std::exception
{
	private:
		DirectiveNotRecognized();
	protected:
		std::vector<std::string>	_line;
		std::string			_ret;
	public:
		DirectiveNotRecognized(std::vector<std::string>& line);
		virtual ~DirectiveNotRecognized() throw();
		const char*	what (void) const throw();
};

class cmbsUnitPrefix : public std::exception
{
	private:
		cmbsUnitPrefix();
	protected:
		std::vector<std::string>	_line;
		std::string					_unitPrefix;
		std::string					_ret;

	public:
		cmbsUnitPrefix(std::vector<std::string>& line);
		virtual ~cmbsUnitPrefix() throw();
		const char*	what (void) const throw();
};

class ElemDefNotRecognized : public std::exception
{
	private:
		ElemDefNotRecognized();
	protected:
		const std::string				_elem;
		const std::string				_expected;
		const std::vector<std::string>	_line;
		const std::string				_defenition;
		std::string						_ret;

	public:
		ElemDefNotRecognized(const std::string& elem, const std::string& expected, std::vector<std::string>& line);
		virtual ~ElemDefNotRecognized() throw();
		const char*	what (void) const throw();
};

class leInvalidMethod : public std::exception //done misschien shit verplaatsen
{
	private:
		leInvalidMethod();
	protected:
		const std::vector<std::string>	_line;
		const std::string				_elem;
		std::string						_ret;

	public:
		leInvalidMethod(std::vector<std::string>& line, std::string elem);
		virtual ~leInvalidMethod() throw();
		const char*	what (void) const throw();
};

class ElemNotANumber : public std::exception
{
	private:
		ElemNotANumber();
	protected:
		const std::vector<std::string>	_line;
		std::string						_ret;
	public:
		ElemNotANumber(std::vector<std::string>& line);
		virtual ~ElemNotANumber() throw();
		const char*	what (void) const throw();
};

#endif
