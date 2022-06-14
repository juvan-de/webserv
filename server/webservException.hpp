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
		std::string			_ret;
	public:
		WrongFile(const std::string filename);
		virtual ~WrongFile() throw();
		const char*	what (void) const throw();
};

class ArgumentIncorrect : public std::exception
{
	private:
		ArgumentIncorrect();
	protected:
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
		std::string						_ret;
	public:
		ElemNotANumber(const std::string& number, const std::vector<std::string>& line);
		virtual ~ElemNotANumber() throw();
		const char*	what (void) const throw();
};

class MissingRootInLocation : public std::exception
{
	private:
		MissingRootInLocation();
	protected:
		std::string						_ret;
	public:
		MissingRootInLocation(const std::string& title);
		virtual ~MissingRootInLocation() throw();
		const char*	what (void) const throw();
};

class RedirWrongStatusCode : public std::exception
{
	private:
		RedirWrongStatusCode();
	protected:
		std::string						_ret;
	public:
		RedirWrongStatusCode(int statucode);
		virtual ~RedirWrongStatusCode() throw();
		const char*	what (void) const throw();
};

class GetWDFailed : public std::exception
{
	protected:
		std::string						_ret;
	public:
		GetWDFailed();
		virtual ~GetWDFailed() throw();
		const char*	what (void) const throw();
};

class RealPathFailed : public std::exception
{
	protected:
		std::string						_ret;
	public:
		RealPathFailed();
		virtual ~RealPathFailed() throw();
		const char*	what (void) const throw();
};

class FileEmpty : public std::exception
{
	protected:
		std::string						_ret;
	public:
		FileEmpty();
		virtual ~FileEmpty() throw();
		const char*	what (void) const throw();
};

class DuplicateNumber : public std::exception
{
	protected:
		std::string						_ret;
	public:
		DuplicateNumber(const std::string& number, const std::vector<std::string>& line);
		virtual ~DuplicateNumber() throw();
		const char*	what (void) const throw();
};

#endif
