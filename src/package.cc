#include "src/package.h"
#include "src/customer.h"
#include "./json_parser.h"
#include <string>

void csci3081::Package::Set_Attributes(const picojson::object &obj) {
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

  if (csci3081::JsonParser::Contains_Key(obj, "weight")) {
    weight_ = csci3081::JsonParser::Get_Double(obj, "weight");
  } else {
    weight_ = 0;
  }
}

/*
* Helper function to send messages to all observers
*
*/
void csci3081::Package::Broadcast(std::string message) {
  for (entity_project::EntityObserver* observer : observers_) {
    // const char* json = "{\"type\": \"notify\",\"value\": "+message+" }";

    picojson::object o;
    o["type"] = picojson::value("notify");
    o["value"] = picojson::value(message);
    picojson::value v = picojson::value(o);

    Package* currentPackage = this;
    Entity * observedEntity = &(*currentPackage);

    observer->OnEvent(v, (*observedEntity));
  }
}
