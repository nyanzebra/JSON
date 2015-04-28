#include <fstream>
#include <iostream>

#include "JSONIO.h"

void std::json::JSONIO::read(const std::string& file_name) {
	ifstream reader;

	reader.open(file_name);

	std::string line;

	if (!reader.is_open()) {
		throw new JSONIOException("file is not open");
	}

	while (getline(reader, line)) {
		if (line[0] != Char::HASH) {
			File_Data.push_back(line);
		}
	}

	reader.close();
}

void std::json::JSONIO::write(const std::string& file_name, const JSONObject& root) {
	ofstream writer;

	writer.open(file_name);

	writer<<root.toString();

	writer.close();
}