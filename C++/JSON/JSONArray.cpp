#include "JSON.h"
#include "Iterator.h"

const std::string std::json::JSONArray::toString(int tabs) const {
	string json_string = opening(std::Char::OPENING_BRACKET, tabs);
	tabs++;
	for (JSON_Vector::const_iterator it = JSON_Array.cbegin(); it != JSON_Array.cend(); ++it) {
		json_string += (*it)->toString(tabs);
		if (std::Iterator::next(it) != JSON_Array.cend()) {
			json_string += std::Char::toString(std::Char::COMMA);
		}
		json_string += std::Char::toString(std::Char::NEW_LINE);
	}
	return json_string + closing(std::Char::CLOSING_BRACKET, tabs);
}
