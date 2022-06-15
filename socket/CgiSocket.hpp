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
	SENT = 2,
	FINISHED = 3
} t_status;

class CgiSocket
{
	private:
		/*--------------------------Member variables--------------------------*/
		t_status					_status;
		bool						_hasBody;
		std::string					_filepath;
		std::vector<std::string>	_envp;
		int							_pipeIn[2];
		int							_pipeOut[2];
		int							_fdIn;
		int							_fdOut;
		std::string					_input;
		std::string					_output;

		std::string			getFilepath(std::string filename, Server server);
		void				mainProcess();
		void				childProccess();
	public:
		/*----------------------------Coplien form----------------------------*/
		~CgiSocket();

		/*--------------------------Member functions--------------------------*/
		CgiSocket(std::string filename, Request request, Server server, sockaddr_in client_struct);
		void				prepareCgi();
		void				executeCgi();
		void				read_from_cgi();
		void				write_to_cgi();
		void				checkError();
		
		int					getFdIn() const { return _fdIn; }
		int					getFdOut() const { return _fdOut; }
		const std::string	getOutput() const { return _output; }
		t_status			getStatus() const { return _status; }
		bool				getBodyStatus() const { return _hasBody; }
		void				setSatus(t_status status) { _status = status; }

	public: /* -Exception- */
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
