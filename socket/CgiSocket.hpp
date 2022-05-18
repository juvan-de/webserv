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

typedef enum e_status
{
	CREATED = 0,
	ADDED = 1,
	FINISHED = 1
} t_status;

class CgiSocket
{
	private:
		/*--------------------------Member variables--------------------------*/
		// int							_fdIn;
		t_status					_status;
		int							_fdOut[2];
		std::string					_input;

		CgiSocket(const CgiSocket &ref);
		CgiSocket& operator=(const CgiSocket &ref);
	public:
		/*----------------------------Coplien form----------------------------*/
		~CgiSocket();

		/*--------------------------Member functions--------------------------*/
		CgiSocket(Request request, Server server, sockaddr_in client_struct);
		void		executeCgi(std::string filepath, std::vector<std::string> envp);
		void		read_cgi();
		int			getFd() const { return _fdOut[0]; }
		void		setSatus(t_status status) { _status = status; }
		t_status	getStatus() const { return _status; }
};

#endif
