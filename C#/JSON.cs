using System;
using System.Collections.Generic;


namespace System {
    namespace JSON {
        public abstract class JSON {
            public JSON this[string key] {
                get {
                    return JSON_Fields[key];
                }
                set {
                    JSON_Fields[key] = value;
                }
            }

            public JSON this[int index] {
                get {
                    return JSON_Array[index];
                }
                set {
                    JSON_Array[index] = value;
                }
            }

            public abstract string asJSONString(int tabs = 0);

            public abstract JSONType getJSONType();

            protected string closing(char closing_container, int tabs) {
                tabs--;
                if (tabs <= 0) {
                    Should_Skip_Opening_Container = true;
                    return "";
                    //return closing_container.ToString() + Input.NEW_LINE.ToString();
                }
                string closing = closing_container.ToString();
                closing = closing.PadLeft(tabs, Input.TAB);
                return closing;
            }

            protected string opening(char opening_container, int tabs) {
                if (Should_Skip_Opening_Container) {
                    Should_Skip_Opening_Container = false;
                    return "";
                }
                if (tabs <= 0) {
                    return opening_container.ToString() + Input.NEW_LINE.ToString();
                }
                string opening = opening_container.ToString();
                opening = opening.PadLeft(tabs, Input.TAB);
                return opening + Input.NEW_LINE.ToString();
            }

            protected Dictionary<string, JSON> JSON_Fields = new Dictionary<string, JSON>();
            protected List<JSON> JSON_Array = new List<JSON>();
            private static bool Should_Skip_Opening_Container = true;
        }

        public class JSONObject : JSON {
            public JSONObject(string name, JSONObject parent) {
                Name = name;
                Parent = parent;
            }

            public JSONObject() {
                Parent = this;
            }

            public JSONObject(string name) {
                Name = name;
                Parent = this;
            }

            public JSONObject getParent() {
                return Parent;
            }

            public override JSONType getJSONType() {
                return JSONType.OBJECT;
            }

            public override string asJSONString(int tabs = 0) {
                string json_string = opening(Input.OPENING_BRACE, tabs);
                tabs++;
                foreach (KeyValuePair<string, JSON> pair in JSON_Fields) {
                    json_string = json_string.PadRight(json_string.Length + tabs - 1, Input.TAB);
                    json_string += Input.DOUBLE_QUOTES.ToString() + pair.Key + Input.DOUBLE_QUOTES.ToString() + Input.COLON.ToString();
                    if (pair.Value.getJSONType() == JSONType.OBJECT || pair.Value.getJSONType() == JSONType.ARRAY) {
                        json_string += Input.NEW_LINE.ToString() + pair.Value.asJSONString(tabs);
                    } else {
                        json_string += pair.Value.asJSONString();
                    }
                    json_string += Input.NEW_LINE.ToString();
                }
                return json_string + closing(Input.CLOSING_BRACE, tabs);
            }

            public bool hasField(string key) {
                return JSON_Fields.ContainsKey(key);
            }

            public void addField(string key, JSON value) {
                JSON_Fields.Add(key, value);
            }

            public void removeField(string key) {
                JSON_Fields.Remove(key);
            }

            public Dictionary<string, JSON>.Enumerator GetEnumerator() {
                return JSON_Fields.GetEnumerator();
            }

            private JSONObject Parent;
            private string Name = "";
        }

        public class JSONArray : JSON {
            public JSONArray() { }

            public void populate(List<JSON> json_array) {
                JSON_Array = json_array;
            }

            public List<JSON>.Enumerator GetEnumerator() {
                return JSON_Array.GetEnumerator();
            }

            public override JSONType getJSONType() {
                return JSONType.ARRAY;
            }

            public override string asJSONString(int tabs = 0) {
                string json_string = opening(Input.OPENING_BRACKET, tabs);
                tabs++;
                for (int i = 0; i < JSON_Array.Count; ++i) {
                    json_string += JSON_Array[i].asJSONString(tabs);
                    if (i != JSON_Array.Count - 1) {
                        json_string += Input.COMMA.ToString();
                    }
                    json_string += Input.NEW_LINE.ToString();
                }
                return json_string + closing(Input.CLOSING_BRACKET, tabs);
            }

            public void add(JSON json) {
                JSON_Array.Add(json);
            }

            public void remove(JSON json) {
                JSON_Array.Remove(json);
            }
        }

        public class JSONField : JSON {
            public JSONField(string value, JSONType type) {
                String_Value = value.Trim(Input.DOUBLE_QUOTES);
                JSON_Type = type;
                if (type == JSONType.ARRAY || type == JSONType.OBJECT) {
                    throw new JSONException("Inappropriate type for JSONField");
                }
            }

            public Object getValue() {
                switch (JSON_Type) {
                    case JSONType.BOOL:
                        return (Object)bool.Parse(String_Value);
                    case JSONType.NUMBER:
                        return (Object)float.Parse(String_Value);
                    case JSONType.STRING:
                        return String_Value;
                    default:
                        throw new JSONException("Inappropriate type for JSONField");
                }
            }

            public override JSONType getJSONType() {
                return JSON_Type;
            }

            public override string asJSONString(int tabs = 0) {
                string json_string = String_Value.PadLeft(tabs + String_Value.Length - 1, Input.TAB);
                if (JSON_Type == JSONType.STRING) {
                    json_string = ("\"" + String_Value + "\"").PadLeft(tabs + String_Value.Length + 1, Input.TAB);
                }

                return json_string;
            }

            private string String_Value = "";
            private JSONType JSON_Type = JSONType.NONE;
        }
    }
}