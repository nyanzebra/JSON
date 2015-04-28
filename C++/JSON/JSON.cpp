#include "JSON.h"

bool std::json::JSON::Should_Skip_Opening_Container = true;

const std::string std::json::JSON::createTabs(const int& tabs = 0) const {
	if (tabs <= 0) {
		return "";
	}
	std::string tabs_string = "";
	for (int i = 0; i < tabs; ++i) {
		tabs_string += std::Char::TAB;
	}
	return tabs_string;
}

const std::string std::json::JSON::opening(const char& opening_container, int tabs = 0) const {
	if (Should_Skip_Opening_Container) {
		Should_Skip_Opening_Container = false;
		return "";
	}
	if (tabs <= 0) {
		return std::Char::toString(opening_container) + std::Char::toString(std::Char::NEW_LINE);
	}

	std::string opening = std::Char::toString(opening_container);
	opening.insert(0, createTabs(tabs - 1));
	opening += std::Char::NEW_LINE;

	return opening;
}

const std::string std::json::JSON::closing(const char& closing_container, int tabs = 0) const {
	tabs--;
	if (tabs <= 0) {
		Should_Skip_Opening_Container = true;
		return "";
	}

	std::string closing = std::Char::toString(closing_container);
	closing.insert(0, createTabs(tabs - 1));

	return closing;
}