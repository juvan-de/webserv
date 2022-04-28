#ifndef CGISOCKET_HPP
# define CGISOCKET_HPP

# include <ClientSocket.hpp>

class CgiSocket : public Socket
{
	private:
		/*--------------------------Member variables--------------------------*/
		int				_fd;
		ClientSocket	*_clisock;

	public:
		/*----------------------------Coplien form----------------------------*/
		CgiSocket();
		CgiSocket(const CgiSocket &ref);
		CgiSocket& operator=(const CgiSocket &ref);
		~CgiSocket();

		/*--------------------------Member functions--------------------------*/

};

#endif
