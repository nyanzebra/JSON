#pragma once

#include <unordered_map>
#include <list>
#include <exception>
#include <iostream>
#include <string>
#include <memory>

#include "Char.h"
#include "String.h"

namespace std {
	namespace json {

		const enum JSONType {
			NONE,
			STRING,
			BOOL,
			NUMBER,
			ARRAY,
			OBJECT,
		};

		class JSONException : runtime_error {
		public:
			JSONException(string message) : runtime_error(message) {}
			virtual const char* what() const throw() {
				string what = "JSONException:\t";
				what += runtime_error::what();
				return what.c_str();
			}
		};

		class JSONIOException : runtime_error {
		public:
			JSONIOException(string message) : runtime_error(message) {}
			virtual const char* what() const throw() {
				string what = "JSONIOException:\t";
				what += runtime_error::what();
				return what.c_str();
			}
		};

		class JSON {
		public:
			virtual const string toString(int tabs = 0) const = 0;
			virtual const JSONType type() const = 0;

			static bool isFieldType(const JSONType& type) {
				return type == JSONType::BOOL || type == JSONType::NUMBER || type == JSONType::STRING;
			}

		protected:
			const string opening(const char& opening_container, int tabs) const;
			const string closing(const char& closing_container, int tabs) const;

			const string createTabs(const int& tabs) const;

			const string keyFormat(const string& key) const {
				return Char::toString(Char::DOUBLE_QUOTES) + key + Char::toString(Char::DOUBLE_QUOTES) + Char::toString(Char::COLON);
			}

		private:
			static bool Should_Skip_Opening_Container;
		};

		typedef unordered_map<string, JSON*> JSON_Map;
		typedef vector<JSON*> JSON_Vector;

		class JSONField : public JSON {
		public:
			JSONField(const string& value, const JSONType& type) : Value(std::String::trim({ Char::DOUBLE_QUOTES }, value)), Type(type) {
				if (!isFieldType(type)) {
					throw new JSONException("Inappropriate type for JSONField");
				}
			}

			template <typename JSONFieldType>
			const JSONFieldType& getValue() const;

			virtual const JSONType type() const {
				return Type;
			}

			virtual const string toString(int tabs = 0) const;

		private:
			string Value = "";
			JSONType Type = JSONType::NONE;
		};

		class JSONObject : public JSON {
		public:
			JSONObject() : Parent(this) {}
			JSONObject(const string& name) : Name(name), Parent(this) {}
			JSONObject(const string& name, JSONObject* parent) : Name(name) {
				Parent = parent;
			}
			~JSONObject() { delete Parent; }

			JSON& operator[] (const string& key) { return *JSON_Fields[key]; }

			virtual const JSONType type() const {
				return JSONType::OBJECT;
			}

			virtual const string toString(int tabs = 0) const;

			const bool hasField(const string& key) const {
				return JSON_Fields.find(key) != JSON_Fields.cend();
			}

			void addField(const string& key, JSON* value) {
				JSON_Fields[key] = value;
			}

			void removeField(const string& key) {
				JSON_Fields.erase(key);
			}

			JSONObject* parent() {
				return Parent;
			}

		private:
			string Name;
			JSONObject* Parent;
			JSON_Map JSON_Fields;
		};

		class JSONArray : public JSON {
		public:
			JSONArray() {}
			~JSONArray() {}

			virtual const JSONType type() const {
				return JSONType::ARRAY;
			}

			virtual const string toString(int tabs = 0) const;

			void add(JSON* json) {
				JSON_Array.push_back(json);
			}

			void remove(const int& position) {
				JSON_Array.erase(JSON_Array.begin() + position);
			}

		private:
			JSON_Vector JSON_Array;
		};
	}
}
