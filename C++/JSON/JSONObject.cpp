#include "JSON.h"
#include "Iterator.h"

const std::string std::json::JSONObject::toString(int tabs) const {
	std::string json_string = opening(std::Char::OPENING_BRACE, tabs);
	tabs++;
	for (JSON_Map::const_iterator it = JSON_Fields.cbegin(); it != JSON_Fields.cend(); ++it) {
		std::string key = it->first;
		json_string.append(createTabs(tabs - 1));
		json_string += keyFormat(key);

		if (it->second->type() == JSONType::OBJECT || it->second->type() == JSONType::ARRAY) {
			json_string += std::Char::toString(std::Char::NEW_LINE) + it->second->toString(tabs);
		} else {
			json_string += it->second->toString();
		}
		if (std::Iterator::next(it) != JSON_Fields.cend()) {
			json_string += std::Char::COMMA;
		}
		json_string += std::Char::NEW_LINE;
	}

	return json_string + closing(std::Char::CLOSING_BRACE, tabs);
}