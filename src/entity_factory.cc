#include "src/entity_factory.h"
#include <iostream>

entity_project::Entity *csci3081::EntityFactory::MakeEntity(
    const picojson::object &obj) {
  if (csci3081::JsonParser::Contains_Key(obj, "type")) {
    if (csci3081::JsonParser::Get_String(obj, "type") == "drone") {
     // std::cout << "Created a Drone" << std::endl;
      csci3081::Drone *drone = new csci3081::Drone();
      drone->Set_Details(obj);
      drone->Set_Attributes(obj);
      /*std::cout << "attributes" << std::endl;
      // std::cout << "  model:    " << model_no_ << std::endl;
      std::cout << "  mass:     " << drone->GetMass() << std::endl;
      std::cout << "  speed:    " << drone->GetMaxSpeed() << std::endl;
      std::cout << "  accel:    " << drone->GetBaseAcceleration() << std::endl;
      std::cout << "  capacity: " << drone->GetMaxCapacity() << std::endl;
      std::cout << "  battery:  " << drone->GetRemainingBattery() << std::endl;
      // std::cout << "set attributes" << std::endl;*/
      if (csci3081::JsonParser::Contains_Key(obj, "speed")) {
        drone->Set_Speed(csci3081::JsonParser::Get_Double(obj, "speed"));
       // std::cout << "Speed: " << drone->GetCurrentSpeed() << std::endl;
      } else {
        drone->Set_Speed(30);
      }

      return drone;
    } else if (csci3081::JsonParser::Get_String(obj, "type") == "customer") {
      csci3081::Customer *cust = new csci3081::Customer();
     // std::cout << "Created a Customer" << std::endl;
      cust->Set_Details(obj);
      cust->Set_Attributes(obj);
      return cust;
    } else if (csci3081::JsonParser::Get_String(obj, "type") == "package") {
      csci3081::Package *package = new csci3081::Package();
     // std::cout << "Created a Package" << std::endl;
      package->Set_Details(obj);
      package->Set_Attributes(obj);
      if (csci3081::JsonParser::Contains_Key(obj, "speed")) {
        package->Set_Speed(csci3081::JsonParser::Get_Double(obj, "speed"));
      } else {
        package->Set_Speed(20);
      }
      return package;
    } else {
      entity_project::Entity *default_entity = new entity_project::Entity();
      return default_entity;
    }
  } else {
    entity_project::Entity *default_entity = new entity_project::Entity();
    return default_entity;
  }
}
