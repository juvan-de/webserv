#include <iostream>
#include <vector>
#include <algorithm>

template <class T>
std::vector<T>	fill(T *arg, int size)
{
	std::vector<T> vec;

	for (int i = 0; i < size; i++)
		vec.push_back(arg[i]);
	return vec;
}

int main()
{
	std::string ports[] = {"8070", "8071", "8072"};
	std::vector<std::string> vec = fill<std::string>(ports, 3);

	std::cout << vec[0] << std::endl;
	return 0;
}