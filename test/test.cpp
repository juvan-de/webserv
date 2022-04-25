#include <iostream>
#include <vector>
#include "testclass.hpp"

int main()
{
	std::vector<testclass> array;
	array.reserve(3);
	for (int i = 0; i < 3; i++)
		array.push_back(new testclass());
	return (0);
}