#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <limits.h>
#include <poll.h>

static void	childProccess(int pipe_in[2], int pipe_out[2])
{
	char buff[999999];
	dup2(pipe_in[0], STDIN_FILENO);
	dup2(pipe_out[1], STDOUT_FILENO);
	dup2(pipe_out[1], STDERR_FILENO);

	read(STDIN_FILENO, buff, 999999);
	std::cout << buff << std::endl;
	// std::cout << "test" << std::endl;
	close(pipe_in[0]);
	close(pipe_in[1]);
	close(pipe_out[0]);
	close(pipe_out[1]);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int main()
{
	int	pipe_in[2];
	int	pipe_out[2];
	char buffer[999999];

	if (pipe(pipe_in))
		std::cout << "ERROR" << std::endl;
	if (pipe(pipe_out))
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		std::cout << "ERROR" << std::endl;
	}
	write(pipe_in[1], "Hello World!", 14);
	std::cout << "wtf" << std::endl;
	pid_t pid = fork();
	if (pid == 0) {
		childProccess(pipe_in, pipe_out);
	}
	else
	{
		read(pipe_out[0], buffer, 999999);
		std::cout << "OUTPUT: [" << std::string(buffer) << "]" << std::endl;
	}
}
