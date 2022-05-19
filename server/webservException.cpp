#include <vector>
#include <string>
#include <iostream>

#include <webservException.hpp>

/* Wrong File */
WrongFile::WrongFile(const std::string filename) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid file name\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "File name: ";
	this->_ret += filename;
}
WrongFile::~WrongFile(void) throw() {}
const char*	WrongFile::what (void) const throw() {return this->_ret.c_str(); }


/* Argument Incorrect */
ArgumentIncorrect::ArgumentIncorrect(const std::vector<std::string>& line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid argument amount\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
ArgumentIncorrect::~ArgumentIncorrect(void) throw() {}
const char*	ArgumentIncorrect::what (void) const throw() {return this->_ret.c_str(); }

/* Missing Closing Bracket */
MissingClosingBracket::MissingClosingBracket(const std::string& location) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Missing closing Bracket for " + location;
}
MissingClosingBracket::~MissingClosingBracket(void) throw () {}
const char*	MissingClosingBracket::what(void) const throw() {return this->_ret.c_str(); }

/* Directive not recognized */
DirectiveNotRecognized::DirectiveNotRecognized(std::vector<std::string>& line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Directive no recognized\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += line[0]; 
	this->_ret += "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
DirectiveNotRecognized::~DirectiveNotRecognized() throw() {}
const char*	DirectiveNotRecognized::what (void) const throw() {return this->_ret.c_str(); }

/* Client max body size Unit Prefix */
cmbsUnitPrefix::cmbsUnitPrefix(std::vector<std::string>& line)
{
	size_t idx = line[1].find_first_not_of("0123456789");
	const std::string unitPrefix = std::string(line[1], idx);
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "Invalid unit prefix\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += unitPrefix; 
	this->_ret += "'";
	this->_ret += COLOR_NORMAL;
	this->_ret += " expected: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'k'/'m'/'g'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
cmbsUnitPrefix::~cmbsUnitPrefix() throw() {}
const char*	cmbsUnitPrefix::what (void) const throw() {return this->_ret.c_str(); }

/* Element defenition not recognized */
ElemDefNotRecognized::ElemDefNotRecognized(const std::string& elem, const std::string& expected, std::vector<std::string>& line) {
	const std::string defenition = line[1];
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += elem + " defenition not recognized\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += defenition; 
	this->_ret += "'";
	this->_ret += COLOR_NORMAL;
	this->_ret += " expected: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += expected;
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "\nline:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
ElemDefNotRecognized::~ElemDefNotRecognized() throw() {}
const char*	ElemDefNotRecognized::what (void) const throw() {return this->_ret.c_str(); }

/* Limit except invalid method */
leInvalidMethod::leInvalidMethod(std::vector<std::string>& line, std::string elem) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Invalid HTTP method\n";
	this->_ret += COLOR_NORMAL;
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'" + elem + "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
leInvalidMethod::~leInvalidMethod() throw() {}
const char*	leInvalidMethod::what (void) const throw() {return this->_ret.c_str(); }

/* Element is not a number */
ElemNotANumber::ElemNotANumber(const std::string& number, const std::vector<std::string>& line) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Element is not a number\n";
	this->_ret += "found: ";
	this->_ret += COLOR_WHITE_BOLD;
	this->_ret += "'";
	this->_ret += number; 
	this->_ret += "'\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "line:";
	for (std::vector<std::string>::const_iterator it = line.begin(); it != line.end(); it++)
		this->_ret += " " + *it;
}
ElemNotANumber::~ElemNotANumber() throw() {}
const char*	ElemNotANumber::what (void) const throw() {return this->_ret.c_str(); }

/* Missing root in Location */
MissingRootInLocation::MissingRootInLocation(const std::string& title) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Root is missing from Locationblock\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "Location block: \'";
	this->_ret += title;
	this->_ret += '\'';
}
MissingRootInLocation::~MissingRootInLocation() throw() {}
const char*	MissingRootInLocation::what (void) const throw() {return this->_ret.c_str(); }

/* Missing root in Location */
RedirWrongStatusCode::RedirWrongStatusCode(int statuscode) {
	this->_ret = COLOR_WHITE_BOLD;
	this->_ret += "Statuscode is not allowed\n";
	this->_ret += COLOR_NORMAL_DIM;
	this->_ret += "Statuscode: ";
	this->_ret += statuscode;
}
RedirWrongStatusCode::~RedirWrongStatusCode() throw() {}
const char*	RedirWrongStatusCode::what (void) const throw() {return this->_ret.c_str(); }
