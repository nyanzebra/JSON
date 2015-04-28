#include "JSON.h"

template <typename JSONFieldType>
const JSONFieldType& std::json::JSONField::getValue() const {
	switch (Type) {
	case JSONType::BOOL:
		return (Value == "true") ? true : false;
	case JSONType::NUMBER:
		return stoi(Value);
	case JSONType::STRING:
		return Value;
	}
}

const std::string std::json::JSONField::toString(int tabs) const {
	std::string json_string = Value;

	if (Type == JSONType::STRING) {
		json_string.insert(0, std::Char::toString(std::Char::DOUBLE_QUOTES));
		json_string.append(std::Char::toString(std::Char::DOUBLE_QUOTES));
	}

	json_string.insert(0, createTabs(tabs - 1));

	return json_string;
}