
#ifndef STATUS_CODES_HPP
# define STATUS_CODES_HPP

# include <string>
# include <map>

class StatusCodes
{
	private:
		std::map<int, std::string>	_statusCodes;
		
		StatusCodes(const StatusCodes& obj);
		const std::string&	IStatusCode(int key);
		StatusCodes();
	public:
		static StatusCodes&	Get()
		{
			static StatusCodes instance;
			return instance;
		};

		static std::string	getStatusCode(int key)
		{
			return Get().IStatusCode(key);
		};
};

#endif
