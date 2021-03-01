#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_
#include <picojson.h>
#include <iostream>
#include <string>

namespace csci3081 {

class JsonParser {
  /**
   * @brief Helper class to parse JSON objects.
   */
 public:
  /**
   * Gets a JSON value.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static const picojson::value &Get_Value(const picojson::object &obj,
                                          std::string key) {
    return obj.find(key)->second;
  }
  /**
   * Gets a JSON object.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static const picojson::object &Get_Object(const picojson::object &obj,
                                            std::string key) {
    return Get_Value(obj, key).get<picojson::object>();
  }
  /**
   * Gets a JSON array.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static const picojson::array &Get_Array(const picojson::object &obj,
                                          std::string key) {
    return Get_Value(obj, key).get<picojson::array>();
  }
  /**
   * Parsea a JSON value to a string.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static std::string Get_String(const picojson::object &obj, std::string key) {
    return Get_Value(obj, key).get<std::string>();
  }
  /**
   * Parses a JSON value to a double.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static double Get_Double(const picojson::object &obj, std::string key) {
    return Get_Value(obj, key).get<double>();
  }
  static float Get_Float(const picojson::object &obj, std::string key) {
    return Get_Value(obj, key).get<float>();
  }

  static int Get_Int(const picojson::object &obj, std::string key) {
    return Get_Value(obj, key).get<double>();
  }
  /**
   * Checks if the JSON object has a certain key.
   * @param[in] obj is a JSON object.
   * @param[in] key is a JSON key.
   */
  static bool Contains_Key(const picojson::object &obj, std::string key) {
    return obj.find(key) != obj.end();
  }
  /**
   * Prints key/value pairs from the JSON object.
   * @param[in] obj is a JSON object.
   */
  static void Print_All(const picojson::object &obj,
                        std::string prefix = "  ") {
    for (picojson::object::const_iterator iterator = obj.begin();
         iterator != obj.end(); iterator++) {
      // std::cout << prefix << iterator->first << ": " << iterator->second <<
      // std::endl;
    }
  }
  /**
   * Parses the object for printing.
   * @param[in] obj is a JSON object.
   * @param[in] prefix is a string that formats the printing.
   */
  static void Print(const picojson::object &obj, std::string prefix = "  ") {
    picojson::value val(obj);
    // std::cout << prefix << val.serialize() << std::endl;
  }
};

}  // namespace csci3081
#endif
