#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>

struct pollfd									new_pollfd(int cli_fd);
std::vector<std::string>						split(std::string line, std::string to_split_on = " \t\n\f\v\r");
void											error_check(int err, std::string msg);
size_t											hex_to_int(std::string hex);
std::string										strtrim(std::string toTrim, std::string trimChar);
bool											doesFileExits(std::string& filename);
std::string										getFileName(const Location& loc);
std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>& locations, std::string request_loc);

#endif
