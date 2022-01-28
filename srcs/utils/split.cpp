#include <string>
#include <vector>

bool	is_in(char c, std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == c)
			return true;
	}
	return false;
}

std::vector<std::string>	split(std::string line, std::string to_split_on = " \t\n\f\v\r")
{
	std::vector<std::string>	vec;
	std::string::iterator it = line.begin();
	std::string::iterator start;
	while (it != line.end())
	{
		while (is_in(*it, to_split_on))
			it++;
		if (it != line.end())
			start = it;
		else
			return vec;
		while (it != line.end() && !is_in(*it, to_split_on))
			it++;
		vec.push_back(line.substr(start - line.begin(), it - start));
	}
	return vec;
}
