#pragma once

#include "JSON.h"

namespace std {
	namespace json {
		class JSONBuilder {
		public:
			JSONBuilder(const vector<string>& data) : Data(data) {}

			JSONObject create();

		private:
			JSONObject* createObject(JSONObject* root);
			JSONArray* createArray();

			const JSONType determineType(const string& value) const;

			const bool hasKey(const string& line) const;
			const bool hasJSONFieldValue(const string& line) const;

			const string extractKey(const string& line) const;
			const string extractJSONFieldValue(const string& line) const;

			vector<string> Data;
			size_t Index = 0;
		};
	}
}