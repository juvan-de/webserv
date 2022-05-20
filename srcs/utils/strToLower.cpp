#include <string>

static void	toLowerUtil(char& c)
{
	c = tolower(c);
}

std::string	strToLower(const std::string& str)
{
	std::string ret(str);
	std::for_each(ret.begin(), ret.end(), toLowerUtil);
	return ret;
}
