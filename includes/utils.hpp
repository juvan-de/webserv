#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>

std::vector<std::string>	split(std::string line, std::string to_split_on = " \t\n\f\v\r");
void						error_check(int err, std::string msg);
#endif
