#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <Server.hpp>

void	parse(char* filename, std::vector<Server>& servers);
void	setFileInDeque(char* filename, std::deque<std::string>& filedeque);

#endif
