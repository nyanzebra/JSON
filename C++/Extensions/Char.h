#ifndef CHAR_H
#define CHAR_H
#include <string>

namespace std {
	namespace Char {
		const char TAB = '\t';
		const char CARRAIAGE_RETURN = '\r';
		const char NEW_LINE = '\n';
		const char DOUBLE_QUOTES = '"';
		const char SINGLE_QUOTES = '\'';
		const char OPENING_BRACE = '{';
		const char CLOSING_BRACE = '}';
		const char OPENING_BRACKET = '[';
		const char CLOSING_BRACKET = ']';
		const int NOT_FOUND = -1;
		const char COMMA = ',';
		const char COLON = ':';
		const char SEMICOLON = ';';
		const char HASH = '#';
		const char WHITESPACE = ' ';

		static string toString(const char& character) {
			string resultant;
			resultant += character;
			return resultant;
		}
	}
}
#endif