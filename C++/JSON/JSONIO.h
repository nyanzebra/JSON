#pragma once

#include <string>
#include <list>

#include "JSON.h"

namespace std {
	namespace json {
		class JSONIO {
		public:
			JSONIO() = default;
			JSONIO(const string& file_name) {}
			~JSONIO() {}

			void read(const string& file_name);
			void write(const string& file_name, const JSONObject& root);

			const vector<string>& data() {
				return File_Data;
			}

		private:
			vector<string> File_Data;
		};
	}
}