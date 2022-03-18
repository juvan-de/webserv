#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <Server.hpp>

void	parse(std::string filename, std::vector<Server>& servers);
void	setFileInDeque(std::string filename, std::deque<std::string>& filedeque);

#endif
