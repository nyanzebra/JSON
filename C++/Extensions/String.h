#include <string>
#include <list>

namespace std {
	namespace String {
		static string trim(const list<char> characters, const string& value) {
			string resultant = "";
			bool should_add_character;
			for (char character : value) {
				should_add_character = true;
				for (char bad_char : characters) {
					if (character == bad_char) {
						should_add_character = false;
						break;
					}
				}
				if (should_add_character) {
					resultant += character;
				}
			}
			return resultant;
		}

		static bool isNumber(const string& s) {
			if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

			char * p;
			strtol(s.c_str(), &p, 10);

			return (*p == 0);
		}

		static string toLower(const string& s) {
			string resultant = "";
			for (char c : s) {
				resultant += tolower(c);
			}
			return resultant;
		}

		static bool isBool(const string& s) {
			if (toLower(s) == "true" || toLower(s) == "false") {
				return true;
			}
			return false;
		}		
	}
}