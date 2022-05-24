
#include <utils.hpp>

std::string	strtrim(std::string toTrim, std::string trimChar)
{
	size_t start = toTrim.find_first_not_of(trimChar);
	size_t end = toTrim.find_last_not_of(trimChar);
	std::string res(toTrim, start, end - start + 1);
	return (res);
}