#include <unistd.h>
#include <limits.h>
#include <string>
#include <iostream>

int	isValidPath(const std::string& root, const std::string& filename)
{
	char	buf[PATH_MAX];

	if (realpath(filename.c_str(), buf) == NULL)
		return (500);
	std::string realpath = std::string(buf);
	if (realpath.compare(0, root.size(), root) == 0)
		return (0);
	return (404);
}
