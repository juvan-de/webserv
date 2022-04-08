#include <string>
#include <iostream>

size_t hex_to_int(std::string hex)
{
	size_t num = 0;
	std::string reference = "0123456789ABCDEF";
	int i = 0;
	while (hex[i])
	{
		num += reference.find(hex[i]);
		num *= 16;
		i++;
	}
	num /= 16;
	return (num);
}