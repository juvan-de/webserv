
#ifndef CONTENT_TYPES_HPP
# define CONTENT_TYPES_HPP

# include <string>
# include <map>

class ContentTypes
{
	private:
		std::map<std::string, std::string>	_contentTypes;

		ContentTypes(const ContentTypes& obj);
		const std::string&	IContentType(const std::string& key);
		ContentTypes();
	public:
		static ContentTypes&	Get()
		{
			static ContentTypes instance;
			return instance;
		};

		static std::string	getContentType(const std::string& key)
		{
			return Get().IContentType(key);
		};
};

#endif