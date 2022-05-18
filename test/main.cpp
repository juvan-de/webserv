#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

// static void	push_string(std::vector<char const*> &env, std::string str)
// {
// 	env.push_back(str.c_str());
// }

std::vector<const char*> vector_to_c_array(const std::vector<std::string>& v) {
	std::vector<const char*> cv;
	cv.reserve(v.size() + 1);
	for (size_t i = 0; i < v.size(); i++)
		cv.push_back(v[i].c_str());
	cv.push_back(NULL);
	return cv;
}

int main()
{
	std::vector<std::string> env;

	env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));
	env.push_back(std::string("QUERY_STRING=value1=Hello&value2=World&value3=wot?"));
	std::cout << std::string(env[1]) << std::endl;

	if (execve(std::string("/Users/ztan/projects/webtest/files/html/Website/cgi-bin/test.php").c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)vector_to_c_array(env).data()) < 0)
		std::cout << "Error: couldnt execv: " << errno << std::endl;
	return 0;
}