#include <string>
#include <vector>

enum EHttpMethods {
	GET,
	POST,
	DELETE
};

class Route {
	public:
		Route();
		~Route();

	private:
		std::string					_route;
		std::vector<EHttpMethods>	_allowedMethods;
		std::string					_httpRedirection;
		std::string					_searchDirectory;
		bool						_directoryListing;
		std::string					_defaultFile;

		// CGI?
};
