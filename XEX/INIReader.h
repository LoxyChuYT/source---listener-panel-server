#include <map>
#include <string>

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
	// Construct INIReader and parse given filename. See ini.h for more info
	// about the parsing.
	INIReader(std::string filename);

	// Return the result of ini_parse(), i.e., 0 on success, line number of
	// first error on parse error, or -1 on file open error.
	int ParseError();

	// Get a string value from INI file, returning default_value if not found.
	std::string Get(std::string section, std::string name,
		std::string default_value);

	// Get an integer (long) value from INI file, returning default_value if
	// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
	long GetInteger(std::string section, std::string name, long default_value);

	// Get a BOOLean value from INI file, returning default_value if not found or if
	// not a valid TRUE/FALSE value. Valid TRUE values are "TRUE", "yes", "on", "1",
	// and valid FALSE values are "FALSE", "no", "off", "0" (not case sensitive).
	BOOL GetBOOLean(std::string section, std::string name, BOOL default_value);

private:
	int _error;
	std::map<std::string, std::string> _values;
	static std::string MakeKey(std::string section, std::string name);
	static int ValueHandler(VOID* user, const char* section, const char* name,
		const char* value);
};

