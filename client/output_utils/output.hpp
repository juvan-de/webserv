#ifndef OUTPUT_HPP
# define OUTPUT_HPP

# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define BOLD "\033[1m"
# define RESET "\033[0m"

# include <string>
# include <iostream>

void	print_divider(std::string color, std::string message);
void	print_line(std::string color);

#endif
