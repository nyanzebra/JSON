#include "JSONBuilder.h"

const std::json::JSONType std::json::JSONBuilder::determineType(const std::string& value) const {
	char first_letter = value[0];
	bool is_number = std::String::isNumber(value);
	bool is_bool = std::String::isBool(value);

	if (is_number) {
		return JSONType::NUMBER;
	} else if (value[0] == std::Char::DOUBLE_QUOTES) {
		return JSONType::STRING;
	} else if (value[0] == std::Char::OPENING_BRACE) {
		return JSONType::OBJECT;
	} else if (value[0] == std::Char::OPENING_BRACKET) {
		return JSONType::ARRAY;
	} else if (is_bool) {
		return JSONType::BOOL;
	} else {
		return JSONType::NONE;
	}
}

const bool std::json::JSONBuilder::hasKey(const std::string& line) const {
	if (line[0] == std::Char::DOUBLE_QUOTES && line.find(std::Char::COLON) != std::string::npos) {
		return true;
	}
	return false;
}

const bool std::json::JSONBuilder::hasJSONFieldValue(const std::string& line) const {
	if (line[line.size()] == std::Char::COLON) {
		return false;
	}
	return true;
}

const std::string std::json::JSONBuilder::extractKey(const std::string& line) const {
	if (hasKey(line)) {
		return line.substr(1, line.find(std::Char::COLON) - 2);
	}
	return "";
}

const std::string std::json::JSONBuilder::extractJSONFieldValue(const std::string& line) const {
	if (hasJSONFieldValue(line)) {
		size_t position = line.find(std::Char::COLON) + 1;
		return std::String::trim({ std::Char::COMMA }, line.substr(position));
	}
	return "";
}

std::json::JSONObject std::json::JSONBuilder::create() {
	JSONObject* root = new JSONObject("root");
	createObject(root);
	return *root;
}

std::json::JSONObject* std::json::JSONBuilder::createObject(JSONObject* root) {
	while (Index < Data.size()) {

		std::string trim_line = String::trim({ Char::TAB, Char::CARRAIAGE_RETURN, Char::NEW_LINE }, Data[Index]);

		std::string key = extractKey(trim_line);
		if (key != "") {
			std::string value = extractJSONFieldValue(Data[Index]);
			if (value != "") {
				JSONType type = determineType(value);

				if (JSON::isFieldType(type)) {
					root->addField(key, new JSONField(value, type));
				}
			}
		}
		Index++;

		trim_line = String::trim({ Char::TAB, Char::CARRAIAGE_RETURN, Char::NEW_LINE }, Data[Index]);
		switch (determineType(trim_line)) {
		case JSONType::ARRAY:
			Index++;
			root->addField(key, createArray());
			break;
		case JSONType::OBJECT:
			Index++;

			JSONObject* child = new JSONObject(key, root);
			root->addField(key, child);
			root = child;

			root = createObject(root);
			break;
		}

		if (Index == Data.size() - 1) {
			break;
		}

		trim_line = String::trim({ Char::TAB, Char::CARRAIAGE_RETURN, Char::NEW_LINE }, Data[Index]);
		if (trim_line[0] == std::Char::CLOSING_BRACE) {
			root = root->parent();
			Index++;
			break;
		}
	}

	return root;
}

std::json::JSONArray* std::json::JSONBuilder::createArray() {
	JSONArray* array = new JSONArray();

	while (Index < Data.size()) {
		string line = std::String::trim({ std::Char::TAB, std::Char::NEW_LINE, std::Char::CARRAIAGE_RETURN, std::Char::COMMA }, Data[Index]);

		switch (determineType(line)) {
		case JSONType::BOOL:
			array->add(new JSONField(line, JSONType::BOOL));
			Index++;
			break;
		case JSONType::NUMBER:
			array->add(new JSONField(line, JSONType::NUMBER));
			Index++;
			break;
		case JSONType::STRING:
			array->add(new JSONField(line, JSONType::STRING));
			Index++;
			break;
		case JSONType::ARRAY:
			Index++;
			array->add(createArray());
			break;
		case JSONType::OBJECT:
			JSONObject* array_object = new JSONObject();
			Index++;
			array->add(createObject(array_object));
			break;
		}
		line = std::String::trim({ std::Char::TAB, std::Char::NEW_LINE, std::Char::CARRAIAGE_RETURN, std::Char::COMMA }, Data[Index]);
		if (line[0] == std::Char::CLOSING_BRACKET) {
			Index++;
			break;
		}
	}
	return array;
}