
// #include "Server.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include "utils.hpp"

// bool	serverStart(std::vector<std::string> config, size_t idx)
// {
// 	if (config[idx] == "server {")
// 		return true;
// 	if (idx + 1 < config.size() && config[idx] = "server" && config[idx + 1] == "{")
// 		return true;
// 	return false;
// }

void	setLocation(std::vector<std::string> config, int* i)
{
	location
	while (*i < config.size())
	{
		std::vector<std::string> split;
		split = split(config[*i]);
		if (split.len() == 0)
			continue ;
		switch(split[0])\{
			case "}":
				return ;
			case "root":
				loc.setRoot();
				break ;
			case "client_max_body_size":
				loc.setMaxBodySize();
				break ;
			case "index":
				loc.setIndex();
				break ;
			case "autoindex":
				loc.setAutoIndex();
				break ;
			case "static_dir":
				loc.setStaticDir();
				break ;
			case "cgi":
				loc.setCgi();
				break ;
			case "limit_except":
				loc.setLimitExcept();
				break ;
			case "upload_store":
				loc.setUploadStore();
				break ;
			case "redir":
				loc.setUploadStore();
				break ;
			default:
				trow();
		}
	}
}

void	setServer(std::vector<std::string> config, int* i)
{
	while (*i < )
	{
		if (config[*i] == "\t{")
			break ;
		
	}
}

void	setServers(std::vector<std::string> config)
{

}

int	main(void)
{
	std::vector<std::string>	file;
	std::string					input;
	std::ifstream				stream;

	file.reserve(48);
	stream.open("config.conf");
	// Server	main(stream);
	while (getline(stream, input))
	{
		file.push_back(input);
	}
	for (size_t i = 0; i < file.size(); i++)
		std::cout << file[i] << std::endl;
	return (0);
}
