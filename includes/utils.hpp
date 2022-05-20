#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>

	struct pollfd				new_pollfd(int cli_fd);
	std::vector<std::string>	split_on_chars(std::string line, std::string to_split_on = " \t\n\f\v\r");
	std::vector<std::string>	split_on_str(std::string line, std::string to_split_on);
	std::string					strToLower(const std::string& str);
	void						error_check(int err, std::string msg);
	size_t 						hex_to_int(std::string hex);
	std::string					strtrim(std::string toTrim, std::string trimChar);
	bool						doesFileExist(const std::string& filename);

	struct cmpCaseInsensitive
	{
		bool operator() (const std::string& x, const std::string& y) const {return strToLower(x)<strToLower(y);}
	};

#endif
