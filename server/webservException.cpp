#include <vector>
#include <string>
#include <iostream>

#include <webservException.hpp>

/* Argument Incorrect */
ArgumentIncorrect::ArgumentIncorrect(const std::vector<std::string>& line) : _line(line) {}
ArgumentIncorrect::~ArgumentIncorrect(void) throw() {}

const char*	ArgumentIncorrect::what (void) const throw()
{
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
}

/* Missing Closing Bracket */
MissingClosingBracket::MissingClosingBracket(const std::string& location) : _location(location) {}
MissingClosingBracket::~MissingClosingBracket(void) throw () {}

const char*	MissingClosingBracket::what(void) const throw()
{
	std::string ret = COLOR_WHITE_BOLD;
		ret += "Missing closing Bracket for " + _location; 
		return ret.c_str();
}

/* Element not Recognized */
ElemNotRecognized::ElemNotRecognized(std::vector<std::string>& line) : _line(line) {}
ElemNotRecognized::~ElemNotRecognized() throw() {}

const char*	ElemNotRecognized::what (void) const throw()
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

/* Client max body size Unit Prefix */
cmbsUnitPrefix::cmbsUnitPrefix(std::vector<std::string>& line) : _line(line)
{
	size_t idx = line[1].find_first_not_of("0123456789");
	this->_unitPrefix = std::string(line[1], idx);
}
cmbsUnitPrefix::~cmbsUnitPrefix() throw() {}

const char*	cmbsUnitPrefix::what (void) const throw()
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

/* Element defenition not recognized */
ElemDefNotRecognized::ElemDefNotRecognized(const std::string& elem, const std::string& expected, std::vector<std::string>& line) : _elem(elem), _expected(expected), _line(line), _defenition(line[1]) {}
ElemDefNotRecognized::~ElemDefNotRecognized() throw() {}

const char*	ElemDefNotRecognized::what (void) const throw()
{
	std::string ret;
	ret += COLOR_WHITE_BOLD;
	ret += _elem + " defenition not recognized\n";
	ret += COLOR_NORMAL;
	ret += "found: ";
	ret += COLOR_WHITE_BOLD;
	ret += "'";
	ret += _defenition; 
	ret += "'";
	ret += COLOR_NORMAL;
	ret += " expected: ";
	ret += COLOR_WHITE_BOLD;
	ret += _expected;
	ret += COLOR_NORMAL_DIM;
	ret += "\nline:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		ret += " " + *it;
	return ret.c_str();
}

/* Limit except invalid method */
leInvalidMethod::leInvalidMethod(std::vector<std::string>& line, std::string elem) : _line(line), _elem(elem) {}
leInvalidMethod::~leInvalidMethod() throw() {}

const char*	leInvalidMethod::what (void) const throw()
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