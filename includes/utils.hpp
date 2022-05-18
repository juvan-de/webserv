#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>

struct pollfd				new_pollfd(int cli_fd);
std::vector<std::string>	split(std::string line, std::string to_split_on = " \t\n\f\v\r");
std::vector<std::string>	split_mc(std::string line, std::string to_split_on);
void						error_check(int err, std::string msg);
size_t 						hex_to_int(std::string hex);
std::string					strtrim(std::string toTrim, std::string trimChar);
bool						doesFileExist(const std::string& filename);
std::string					itoa(unsigned int num, int base);
#endif
