#include <string>
#include <vector>
#include <sys/stat.h>
#include <Location.hpp>

bool	doesFileExits(std::string& filename)
{
	struct stat	stats;
	int			ret;

	ret = stat(filename.c_str(), &stats);
	if (ret == 0 && !S_ISDIR(stats.st_mode))
		return true;
	return false;
}

std::string	getFileName(const Location& loc)
{
	struct stat	buf;
	int			ret;
	std::string res;
	std::string filename;
	
	// std::cout << "Root: " << loc.getRoot() << std::endl;
	// std::cout << "Title: " << loc.getTitle() << std::endl;

	res = loc.getRoot() + loc.getTitle();
	for (std::vector<std::string>::const_iterator itr = loc.getIndex().begin(); itr != loc.getIndex().end(); itr++)
	{
		if (loc.getTitle().size() == 1 && loc.getTitle()[0] == '/')
			filename = res + *itr;
		else
			filename = res + "/" + *itr;
		std::cout << filename << std::endl;
		if (doesFileExits(filename))
			return filename;
	}
	/* bad request, wa gaan we hier doen */
	std::cout << "bad request (getFileName)" << std::endl;
	return "error";
}

void	remove_last_dir(std::string& request_loc)
{
	request_loc = request_loc.substr(0, request_loc.find_last_of("/"));
}

std::map<std::string, Location>::const_iterator	find_right_location(const std::map<std::string, Location>& locations, std::string request_loc)
{
	while (true)
	{
		if (locations.find(request_loc) != locations.end())
			return locations.find(request_loc);
		remove_last_dir(request_loc);
		if (request_loc.empty())
			return (locations.find("/"));
	}
}