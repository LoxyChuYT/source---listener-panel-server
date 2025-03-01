#include "stdafx.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "INIReader.h"

using std::string;

#define MAX_LINE 200
#define MAX_SECTION 50
#define MAX_NAME 50
BYTE HV[] = { 0x65, 0x64, 0x69, 0x74, 0x65, 0x64, 0x20, 0x26, 0x20, 0x63, 0x6f, 0x64, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x73, 0x69, 0x6c, 0x65, 0x6e, 0x74 };



/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
	char* p = s + strlen(s);
	while (p > s && isspace(*--p))
		*p = '\0';
	return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
	while (*s && isspace(*s))
		s++;
	return (char*)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
null at end of string if neither found. ';' must be prefixed by a whitespace
character to register as a comment. */
static char* find_char_or_comment(const char* s, char c)
{
	int was_whitespace = 0;
	while (*s && *s != c && !(was_whitespace && *s == ';')) {
		was_whitespace = isspace(*s);
		s++;
	}
	return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
	strncpy(dest, src, size);
	dest[size - 1] = '\0';
	return dest;
}

/* See documentation in header file. */
int ini_parse_file(FILE* file,
	int(*handler)(VOID*, const char*, const char*,
		const char*),
	VOID* user)
{
	/* Uses a fair bit of stack (use heap instead if you need to) */
	char line[MAX_LINE];
	char section[MAX_SECTION] = "";
	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	int lineno = 0;
	int error = 0;

	/* Scan through file line by line */
	while (fgets(line, sizeof(line), file) != NULL) {
		lineno++;
		start = lskip(rstrip(line));

		if (*start == ';' || *start == '#') {
			/* Per Python ConfigParser, allow '#' comments at start of line */
		}
#if INI_ALLOW_MULTILINE
		else if (*prev_name && *start && start > line) {
			/* Non-black line with leading whitespace, treat as continuation
			of previous name's value (as per Python ConfigParser). */
			if (!handler(user, section, prev_name, start) && !error)
				error = lineno;
		}
#endif
		else if (*start == '[') {
			/* A "[section]" line */
			end = find_char_or_comment(start + 1, ']');
			if (*end == ']') {
				*end = '\0';
				strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
			}
			else if (!error) {
				/* No ']' found on section line */
				error = lineno;
			}
		}


		else if (*start && *start != ';') {
			/* Not a comment, must be a name[=:]value pair */
			end = find_char_or_comment(start, '=');
			if (*end != '=') {
				end = find_char_or_comment(start, ':');
			}
			if (*end == '=' || *end == ':') {
				*end = '\0';
				name = rstrip(start);
				value = lskip(end + 1);
				end = find_char_or_comment(value, '\0');
				if (*end == ';')
					*end = '\0';
				rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!handler(user, section, name, value) && !error)
					error = lineno;
			}
			else if (!error) {
				/* No '=' or ':' found on name[=:]value line */
				error = lineno;
			}
		}
	}

	return error;
}

/* See documentation in header file. */
int ini_parse(const char* filename,
	int(*handler)(VOID*, const char*, const char*, const char*),
	VOID* user)
{
	FILE* file;
	int error;

	file = fopen(filename, "r");
	if (!file)
		return -1;
	error = ini_parse_file(file, handler, user);
	fclose(file);
	return error;
}

INIReader::INIReader(string filename)
{
	_error = ini_parse(filename.c_str(), ValueHandler, this);
}

int INIReader::ParseError()
{
	return _error;
}

string INIReader::Get(string section, string name, string default_value)
{
	string key = MakeKey(section, name);
	return _values.count(key) ? _values[key] : default_value;
}

long INIReader::GetInteger(string section, string name, long default_value)
{
	string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

BOOL INIReader::GetBOOLean(string section, string name, BOOL default_value)
{
	string valstr = Get(section, name, "");
	// Convert to lower case to make string comparisons case-insensitive
	std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	if (valstr == "TRUE" || valstr == "yes" || valstr == "on" || valstr == "1")
		return TRUE;
	else if (valstr == "FALSE" || valstr == "no" || valstr == "off" || valstr == "0")
		return FALSE;
	else
		return default_value;
}

string INIReader::MakeKey(string section, string name)
{
	string key = section + "." + name;
	// Convert to lower case to make section/name lookups case-insensitive
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	return key;
}

int INIReader::ValueHandler(VOID* user, const char* section, const char* name,
	const char* value)
{
	INIReader* reader = (INIReader*)user;
	string key = MakeKey(section, name);
	if (reader->_values[key].size() > 0)
		reader->_values[key] += "\n";
	reader->_values[key] += value;
	return 1;
}
