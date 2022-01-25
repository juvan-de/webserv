
#include "Server.hpp"

int	main(int ac, char **av)
{
	std::string input;
	std::ifstream stream;
	stream.open("config.conf");
	Server	main(stream);
	while (getline(stream, input))
	{
		std::cout << input << std::endl;
	}
	return (0);
}