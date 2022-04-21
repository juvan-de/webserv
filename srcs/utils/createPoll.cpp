#include <utils.hpp>
#include <poll.h> // polling

struct pollfd	new_pollfd(int cli_fd)
{
	struct pollfd new_fd;
	int flags;

	new_fd.fd = cli_fd;
	new_fd.events = POLLIN | POLLOUT;
	return new_fd;
}
