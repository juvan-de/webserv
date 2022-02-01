
# include <iostream>
# include <fstream>
# include <vector>
// # include "utils.hpp"
#include "Location.hpp"

// template<class T>
// void	parseLoop(T& block, std::deque<std::string>& config)
// {
// 	while (!config.empty())
// 	{
// 		std::string& line = config.front();
// 		config.pop_front();
// 		std::vector<std::string> splitted = split(line);
// 		fptr setfunc = block.switchcase(splitted[0]);
// 		if (setfunc == NULL)
// 			return ;
// 		block.(*setfunc)(splitted);
// 	}
// }

int main(void)
{
	Location loc;

	std::cout << loc << std::endl;
}

