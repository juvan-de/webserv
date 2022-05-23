#include <string>
#include <vector>
#include <iostream>

static bool	is_in(char c, std::string str)
{
	return (str.find(c) != std::string::npos);
}

std::vector<std::string>	split_on_chars(std::string line, std::string to_split_on)
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

std::vector<std::string>	split_on_str(std::string line, std::string to_split_on)
{
	std::vector<std::string>	vec;
	size_t						pos;
	size_t						start = 0;

	if (line.size() == 0)
		return vec;
	while ((pos = line.find(to_split_on, start)) != std::string::npos)
	{
		if (pos != start)
			vec.push_back(line.substr(start, pos - start));
		start = pos + to_split_on.size();
	}
	if (line.size() != start)
		vec.push_back(line.substr(start, pos - start));
	return vec;
}
