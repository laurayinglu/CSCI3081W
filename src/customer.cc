#include "src/customer.h"
#include "src/json_parser.h"

void csci3081::Customer::Set_Attributes(const picojson::object &obj) {
  if (csci3081::JsonParser::Contains_Key(obj, "id")) {
    id_ = csci3081::JsonParser::Get_Int(obj, "id");
  }
  if (csci3081::JsonParser::Contains_Key(obj, "name")) {
    name_ = csci3081::JsonParser::Get_String(obj, "name");
  }
  if (csci3081::JsonParser::Contains_Key(obj, "radius")) {
    radius_ = csci3081::JsonParser::Get_Double(obj, "radius");
  }
  if (csci3081::JsonParser::Contains_Key(obj, "position")) {
    for (int i = 0; i < 3; i++) {
      position_[i] = 0;
    }
    for (int i = 0;
         i < fmin(static_cast<int>(JsonParser::Get_Array(obj, "position").size()), 3);
         i++) {
      position_[i] = JsonParser::Get_Array(obj, "position")[i].get<double>();
    }
  }
  if (csci3081::JsonParser::Contains_Key(obj, "direction")) {
    for (int i = 0; i < 3; i++) {
      direction_[i] = 0;
    }
    for (int i = 0;
         i < fmin(static_cast<int>(JsonParser::Get_Array(obj, "direction").size()), 3);
         i++) {
      direction_[i] = JsonParser::Get_Array(obj, "direction")[i].get<double>();
    }
  }
}
