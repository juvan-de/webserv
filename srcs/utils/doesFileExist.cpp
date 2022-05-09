#include <sys/stat.h> // stat struct
#include <string>

bool	doesFileExist(const std::string& filename)
{
	struct stat	stats;
	int			ret;

	ret = stat(filename.c_str(), &stats);
	if (ret == 0 && !S_ISDIR(stats.st_mode))
		return true;
	return false;
}
