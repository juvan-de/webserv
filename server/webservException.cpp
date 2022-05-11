#include <vector>
#include <string>
#include <iostream>

#include <webservException.hpp>

/* Wrong File */
WrongFile::WrongFile(const std::string filename) : _filename(filename) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid file name\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "File name: ";
	this->_ret += this->_filename;
}
WrongFile::~WrongFile(void) throw() {}
const char*	WrongFile::what (void) const throw() {return this->_ret.c_str(); }


/* Argument Incorrect */
ArgumentIncorrect::ArgumentIncorrect(const std::vector<std::string>& line) : _line(line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid argument amount\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;
}
ArgumentIncorrect::~ArgumentIncorrect(void) throw() {}
const char*	ArgumentIncorrect::what (void) const throw() {return this->_ret.c_str(); }

/* Missing Closing Bracket */
MissingClosingBracket::MissingClosingBracket(const std::string& location) : _location(location) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Missing closing Bracket for " + _location;
}
MissingClosingBracket::~MissingClosingBracket(void) throw () {}
const char*	MissingClosingBracket::what(void) const throw() {return this->_ret.c_str(); }

/* Element not Recognized */
DirectiveNotRecognized::DirectiveNotRecognized(std::vector<std::string>& line) : _line(line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Directive no recognized\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += _line[0]; 
	this->_ret += "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;
}
DirectiveNotRecognized::~DirectiveNotRecognized() throw() {}
const char*	DirectiveNotRecognized::what (void) const throw() {return this->_ret.c_str(); }

/* Client max body size Unit Prefix */
cmbsUnitPrefix::cmbsUnitPrefix(std::vector<std::string>& line) : _line(line)
{
	size_t idx = line[1].find_first_not_of("0123456789");
	this->_unitPrefix = std::string(line[1], idx);
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "Invalid unit prefix\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += _unitPrefix; 
	this->_ret += "'";
	this->_ret += COLOR_NORMAL;
	this->_ret += " expected: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'k'/'m'/'g'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;
}
cmbsUnitPrefix::~cmbsUnitPrefix() throw() {}
const char*	cmbsUnitPrefix::what (void) const throw() {return this->_ret.c_str(); }

/* Element defenition not recognized */
ElemDefNotRecognized::ElemDefNotRecognized(const std::string& elem, const std::string& expected, std::vector<std::string>& line) : _elem(elem), _expected(expected), _line(line), _defenition(line[1]) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += _elem + " defenition not recognized\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += _defenition; 
	this->_ret += "'";
	this->_ret += COLOR_NORMAL;
	this->_ret += " expected: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += _expected;
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "\nline:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;
}
ElemDefNotRecognized::~ElemDefNotRecognized() throw() {}
const char*	ElemDefNotRecognized::what (void) const throw() {return this->_ret.c_str(); }

/* Limit except invalid method */
leInvalidMethod::leInvalidMethod(std::vector<std::string>& line, std::string elem) : _line(line), _elem(elem) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid HTTP method\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'" + this->_elem + "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;
}
leInvalidMethod::~leInvalidMethod() throw() {}
const char*	leInvalidMethod::what (void) const throw() {return this->_ret.c_str(); }

/* Element is not a number */
ElemNotANumber::ElemNotANumber(std::vector<std::string>& line) : _line(line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Element is not a number\n";
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += _line[1]; 
	this->_ret += "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = this->_line.begin(); it != this->_line.end(); it++)
		this->_ret += " " + *it;

}
ElemNotANumber::~ElemNotANumber() throw() {}
const char*	ElemNotANumber::what (void) const throw() {return this->_ret.c_str(); }