#ifndef CGISOCKET_HPP
# define CGISOCKET_HPP

# include <Socket.hpp>

class CgiSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		std::string		_output;

	public:
		/*----------------------------Coplien form----------------------------*/
		CgiSocket(int fd);
		// CgiSocket(const CgiSocket &ref);
		// CgiSocket& operator=(const CgiSocket &ref);
		// ~CgiSocket();

		/*--------------------------Member functions--------------------------*/


};

#endif
