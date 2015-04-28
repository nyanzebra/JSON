using System;
using System.IO;
using System.Collections.Generic;


namespace System {
    public static class StringExtension {
        public static bool isNumber(this string value) {
            foreach (char c in value) {
                if (!Char.IsNumber(c)) {
                    return false;
                }
            }
            return true;
        }

        public static bool isBoolean(this string value) {
            return value.ToLower().Equals("true") || value.ToLower().Equals("false");
        }
    }

    public static class Input {
        public const char TAB = '\t';
        public const char CARRIAGE_RETURN = '\r';
        public const char NEW_LINE = '\n';
        public const char DOUBLE_QUOTES = '"';
        public const char SINGLE_QUOTES = '\'';
        public const char OPENING_BRACE = '{';
        public const char CLOSING_BRACE = '}';
        public const char OPENING_BRACKET = '[';
        public const char CLOSING_BRACKET = ']';
        public const int NOT_FOUND = -1;
        public const char COMMA = ',';
        public const char COLON = ':';
        public const char SEMICOLON = ';';
        public const char HASH = '#';
        public const char WHITESPACE = ' ';
    }
}

namespace System {
    namespace JSON {
        public enum JSONType { NONE, STRING, BOOL, NUMBER, ARRAY, OBJECT }

        public class JSONException : Exception {
            public JSONException(string message)
                : base(message) {
                Console.Write("JSONException:\t");
            }
        }

        public class JSONIOException : Exception {
            public JSONIOException(string message)
                : base(message) {
                Console.Write("JSONIOException:\t");
            }
        }

        public class JSONIO {
            public JSONIO(string file_name) {
                read(file_name);
            }

            public JSONIO() {}

            public void read(string file_name) {
                StreamReader stream_reader = new StreamReader(file_name);
                while (!stream_reader.EndOfStream) {
                    string line = stream_reader.ReadLine();
                    if (line.IndexOf(Input.HASH) == Input.NOT_FOUND) {
                        if (!line.Equals("")) {
                            File_Data.Add(line.Trim(Input.TAB, Input.NEW_LINE, Input.CARRIAGE_RETURN));
                        }
                    }
                }
                try {
                    stream_reader.Dispose();
                } catch (Exception e) {
                    Console.WriteLine(e.Message);
                }
            }

            public void write(string file_name, JSON root) {
                StreamWriter stream_writer = new StreamWriter(file_name);
                stream_writer.WriteLine(root.asJSONString());
                try {
                    stream_writer.Dispose();
                } catch (Exception e) {
                    Console.WriteLine(e.Message);
                }
            }

            private void handleFields(ref JSONObject root) {
                if (hasKey(File_Data[Index])) {
                    Key = extractKey(File_Data[Index]);
                    if (hasValue(File_Data[Index])) {
                        string value = extractFieldValue(File_Data[Index]);
                        JSONType type = determineType(value);

                        if (isFieldType(type)) {
                            root.addField(Key, new JSONField(value, type));
                        }
                    }
                    Index++;
                }
            }

            private bool isFieldType(JSONType type) {
                return type == JSONType.BOOL || type == JSONType.NUMBER || type == JSONType.STRING;
            }

            private JSONType determineType(string value) {
                if (value.isNumber()) {
                    return JSONType.NUMBER;
                } else if (value[0] == Input.DOUBLE_QUOTES) {
                    return JSONType.STRING;
                } else if (value[0] == Input.OPENING_BRACE) {
                    return JSONType.OBJECT;
                } else if (value[0] == Input.OPENING_BRACKET) {
                    return JSONType.ARRAY;
                } else if (value.isBoolean()) {
                    return JSONType.BOOL;
                } else {
                    return JSONType.NONE;
                }
            }

            private void handleObjects(ref JSONObject json_object) {
                Index++;
                moveRootDownToChild(ref json_object);
                createJSON(ref json_object);
            }

            private void moveRootDownToChild(ref JSONObject json_object) {
                JSONObject child = new JSONObject(Key, json_object);
                json_object.addField(Key, child);
                json_object = child;
            }

            private void handleArrays(string key, ref JSONObject root) {
                Index++;
                root.addField(key, createJSONArray());
            }

            public JSONObject createJSON(ref JSONObject root) {
                while (Index < File_Data.Count) {
                    handleFields(ref root);

                    switch (determineType(File_Data[Index])) {
                        case JSONType.ARRAY:
                            handleArrays(Key, ref root);
                            break;
                        case JSONType.OBJECT:
                            handleObjects(ref root);
                            break;
                    }

                    if (Index == File_Data.Count) {
                        break;
                    }
                    if (File_Data[Index][0] == Input.CLOSING_BRACE) {
                        root = root.getParent();
                        Key = "";
                        Index++;
                        break;
                    }
                }

                return root;
            }

            private JSONArray createJSONArray() {
                JSONArray array = new JSONArray();
                while (Index < File_Data.Count) {
                    string line = File_Data[Index].Trim(Input.COMMA);
                    switch (determineType(line)) {
                        case JSONType.BOOL:
                            array.add(new JSONField(line, JSONType.BOOL));
                            Index++;
                            break;
                        case JSONType.NUMBER:
                            array.add(new JSONField(line, JSONType.NUMBER));
                            Index++;
                            break;
                        case JSONType.STRING:
                            array.add(new JSONField(line, JSONType.STRING));
                            Index++;
                            break;
                        case JSONType.ARRAY:
                            Index++;
                            array.add(createJSONArray());
                            break;
                        case JSONType.OBJECT:
                            JSONObject array_object = new JSONObject();
                            Index++;
                            array.add(createJSON(ref array_object));
                            break;
                    }
                    if (File_Data[Index][0] == Input.CLOSING_BRACKET) {
                        Index++;
                        Key = "";
                        break;
                    }
                }
                return array;
            }

            private string extractFieldValue(string line) {
                return line.Substring(line.IndexOf(Input.COLON) + 1).Trim(Input.COMMA);
            }

            private bool hasValue(string line) {
                if (line.EndsWith(Input.COLON.ToString())) {
                    return false;
                }
                return true;
            }

            private bool hasKey(string line) {
                if (line[0] == Input.DOUBLE_QUOTES && line.IndexOf(Input.COLON) != Input.NOT_FOUND) {
                    return true;
                }
                return false;
            }

            private string extractKey(string line) {
                return line.Substring(0, line.IndexOf(Input.COLON)).Trim(Input.DOUBLE_QUOTES);
            }

            private int Index = 0;
            private string Key = "";
            private List<string> File_Data = new List<string>();
        }
    }
}
