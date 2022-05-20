#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>

	std::vector<std::string>	split_on_chars(std::string line, std::string to_split_on = " \t\n\f\v\r");
	std::vector<std::string>	split_on_str(std::string line, std::string to_split_on);
	std::string					strToLower(const std::string& str);
	bool						doesFileExist(const std::string& filename);

	struct cmpCaseInsensitive
	{
		bool operator() (const std::string& x, const std::string& y) const {return strToLower(x)<strToLower(y);}
	};

#endif
