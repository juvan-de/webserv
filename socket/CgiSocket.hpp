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
	FINISHED = 2
} t_status;

class CgiSocket
{
	private:
		/*--------------------------Member variables--------------------------*/
		t_status					_status;
		int							_pipeIn[2];
		int							_pipeOut[2];
		int							_fdIn;
		int							_fdOut;
		std::string					_output;

		std::string			getFilepath(Server server, Request request);
	public:
		/*----------------------------Coplien form----------------------------*/
		~CgiSocket();

		/*--------------------------Member functions--------------------------*/
		CgiSocket(Request request, Server server, sockaddr_in client_struct);
		void				executeCgi(std::string filepath, std::vector<std::string> envp);
		void				read_or_write_cgi(int fd);
		void				checkError();
		
		int					getFdIn() const { return _fdIn; }
		int					getFdOut() const { return _fdOut; }
		const std::string	getInput() const { return _output; }
		void				setSatus(t_status status) { _status = status; }
		t_status			getStatus() const { return _status; }

	private: /* -Exception- */
		class CgiException : public std::exception
		{
			private:
				int _code;
			public:
			CgiException(int code) : _code(code) {}
			int	getError(void) const throw()
			{
				return (this->_code);
			}
		};
};

#endif
