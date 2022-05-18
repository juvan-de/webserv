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

std::vector<std::string>	split(std::string line, std::string to_split_on)
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

std::vector<std::string>	split_mc(std::string line, std::string to_split_on)
{
	std::vector<std::string>	vec;
	size_t i = 0;
	size_t start = 0;
	while (i < line.size())
	{
		i = line.find(to_split_on, i);
		if (i == std::string::npos)
		{
			vec.push_back(line.substr(start));
			return (vec);
		}
		vec.push_back(line.substr(start, i));
		i += to_split_on.size();
		start = i;
	}
	return (vec);
}
