#include "output.hpp"

void	print_divider(std::string color, std::string message)
{
	std::cout << BOLD << color << "--------------------------------------" <<
		std::endl << message << std::endl <<
		"--------------------------------------" << RESET << std::endl;
}

void	print_line(std::string color)
{
	std::cout << BOLD << color << "--------------------------------------"
		<< RESET << std::endl;
}
