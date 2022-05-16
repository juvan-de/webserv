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

class CgiSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::string					_output;
		std::vector<char const*>	_env;

	public:
		/*----------------------------Coplien form----------------------------*/
		// CgiSocket(const CgiSocket &ref);
		// CgiSocket& operator=(const CgiSocket &ref);
		// ~CgiSocket();

		/*--------------------------Member functions--------------------------*/
		CgiSocket(Request request, Server server, sockaddr_in client_struct);
		void	executeCgi(std::string filepath);
};

#endif
