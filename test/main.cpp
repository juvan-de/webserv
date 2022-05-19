#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <limits.h>

// static void	push_string(std::vector<char const*> &env, std::string str)
// {
// 	env.push_back(str.c_str());
// }

// std::vector<const char*> vector_to_c_array(const std::vector<std::string>& v) {
// 	std::vector<const char*> cv;
// 	cv.reserve(v.size() + 1);
// 	for (size_t i = 0; i < v.size(); i++)
// 		cv.push_back(v[i].c_str());
// 	cv.push_back(NULL);
// 	return cv;
// }

int main()
{
	char *str;
	if ((str = realpath("../test/main.cpp", NULL)) == NULL) {
		std::cout << "Failed: " << errno << std::endl;
		exit(1);
	}
	std::cout << std::string(str) << std::endl;
	free(str);
	return (0);
}