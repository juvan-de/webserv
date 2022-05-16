#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

static void	push_string(std::vector<char const*> &env, std::string str)
{
	env.push_back(str.c_str());
}

int main()
{
	std::vector<char const*> env;

	push_string(env, "GATEWAY_INTERFACE=CGI/1.1");
	push_string(env, std::string("QUERY_STRING=value1=Hello&value2=World&value3=wot?"));

	if (execve(std::string("/Users/ztan/projects/webtest/files/html/Website/cgi-bin/test.php").c_str(), (char *const *)std::vector<char const*>().data(), (char *const *)env.data()) < 0)
		std::cout << "Error: couldnt execv: " << errno << std::endl;
	return 0;
}