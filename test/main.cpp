#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <limits.h>
#include <poll.h>


void	ifStatement()
{
	short val = (POLLIN | POLLHUP);

	if (val & POLLIN) {
		std::cout << "POLLIN" << std::endl;
	}
	else if (val & POLLHUP) {
		std::cout << "POLLHUP" << std::endl;
	}
	else if (val & (POLLIN | POLLHUP)) {
		std::cout << "POLLIN | POLLHUP" << std::endl;
	}
}

void	switchCase()
{
	short val = (POLLIN | POLLHUP);

	switch (val)
	{
	case POLLSTANDARD & POLLIN:
		std::cout << "POLLIN" << std::endl;
		break;
	case POLLSTANDARD & POLLOUT:
		std::cout << "POLLHUP" << std::endl;
		break;
	case POLLSTANDARD & (POLLIN | POLLOUT):
		std::cout << "POLLIN | POLLHUP" << std::endl;
		break;
	}
}

int main()
{
	ifStatement();
	ifStatement();
	ifStatement();
	// switchCase();
	// switchCase();
	// switchCase();
	return (0);
}