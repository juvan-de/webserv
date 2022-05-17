#ifndef CGISOCKET_HPP
# define CGISOCKET_HPP

# include <Socket.hpp>
# include <iostream>
# include <vector>
# include <map>
# include <Request.hpp>
# include <Server.hpp>
# include <netinet/in.h>
# include <arpa/inet.h>

class CgiSocket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int							_fdIn;
		int							_fdOut;
		std::string					_input;

		CgiSocket(const CgiSocket &ref);
		CgiSocket& operator=(const CgiSocket &ref);
	public:
		/*----------------------------Coplien form----------------------------*/
		~CgiSocket();

		/*--------------------------Member functions--------------------------*/
		CgiSocket(Request request, Server server, sockaddr_in client_struct);
		void	executeCgi(std::string filepath, std::vector<std::string> envp);
		void	read_cgi();
		int		getFd() const { return _fdOut; }
};

#endif
