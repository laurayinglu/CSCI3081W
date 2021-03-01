#include "src/drone.h"
#include <string>
#include "./json_parser.h"

void csci3081::Drone::Set_Attributes(const picojson::object& obj) {
  if (csci3081::JsonParser::Contains_Key(obj, "id")) {
    id_ = csci3081::JsonParser::Get_Int(obj, "id");
  }

  if (csci3081::JsonParser::Contains_Key(obj, "name")) {
    name_ = csci3081::JsonParser::Get_String(obj, "name");
  } else {
    name_ = "drone";
  }

  if (csci3081::JsonParser::Contains_Key(obj, "radius")) {
    radius_ = csci3081::JsonParser::Get_Double(obj, "radius");
  }

  if (csci3081::JsonParser::Contains_Key(obj, "position")) {
    for (int i = 0; i < 3; i++) {
      position_[i] = 0;
    }
    for (int i = 0;
         i <
         fmin(static_cast<int>(JsonParser::Get_Array(obj, "position").size()),
              3);
         i++) {
      position_[i] = JsonParser::Get_Array(obj, "position")[i].get<double>();
    }
  }
  if (csci3081::JsonParser::Contains_Key(obj, "direction")) {
    for (int i = 0; i < 3; i++) {
      direction_[i] = 0;
    }
    for (int i = 0;
         i <
         fmin(static_cast<int>(JsonParser::Get_Array(obj, "direction").size()),
              3);
         i++) {
      direction_[i] = JsonParser::Get_Array(obj, "direction")[i].get<double>();
    }
  }

  // added for iteration 2 priority 3
  if (csci3081::JsonParser::Contains_Key(obj, "model")) {
    // std::cout << "DRONE.CC found model in json" << std::endl;
    model_no_ = csci3081::JsonParser::Get_String(obj, "model");
  } else {
    // std::cout << "DRONE.CC model not found in json, setting default" <<
    // std::endl;
    model_no_ = "Q-36-01";
  }

  std::ifstream dronefile;
  dronefile.open("data/planet-x.csv");
  if (!dronefile.is_open()) {
    std::cout << "could not open file";
    return;
  }

  mass_ = 50;
  max_speed_ = 60;
  speed_ = max_speed_;
  accel_ = 4;
  max_capacity_ = 10;
  curr_capacity_ = max_capacity_;
  battery_capacity_ = 300;
  remaining_battery_ = battery_capacity_;
  std::vector<std::string> model_names, columns;
  std::vector<float> masses, speeds, accels, weights, batteries;
  std::string curr_model;
  std::string curr_mass;
  std::string curr_speed;
  std::string curr_accel;
  std::string curr_weight;
  std::string curr_battery;
  std::string temp;

  for (int i = 0; i < 5; i++) {
    std::getline(dronefile, temp, ',');
    columns.push_back(temp);
  }
  std::getline(dronefile, temp, '\n');
  columns.push_back(temp);

  while (dronefile.peek() != EOF) {
    for (int i = 0; i < 5; i++) {
      if (columns.at(i) == "Mass (kg)") {
        std::getline(dronefile, temp, ',');
        masses.push_back(std::stof(temp));
      } else if (columns.at(i) == "Max Speed (km/h)") {
        std::getline(dronefile, temp, ',');
        speeds.push_back(std::stof(temp));
      } else if (columns.at(i) == "Base Acceleration (m/s^2)") {
        std::getline(dronefile, temp, ',');
        accels.push_back(std::stof(temp));
      } else if (columns.at(i) == "WeightCapacity (kg)") {
        std::getline(dronefile, temp, ',');
        weights.push_back(std::stof(temp));
      } else if (columns.at(i) == "Base Battery Capacity (seconds)") {
        std::getline(dronefile, temp, ',');
        batteries.push_back(std::stof(temp));
      } else if (columns.at(i) == "Model #") {
        std::getline(dronefile, temp, ',');
        model_names.push_back(temp);
      }
    }
    if (columns.at(5) == "Mass (kg)") {
      std::getline(dronefile, temp, '\n');
      masses.push_back(std::stof(temp));
    } else if (columns.at(5) == "Max Speed (km/h)") {
      std::getline(dronefile, temp, '\n');
      speeds.push_back(std::stof(temp));
    } else if (columns.at(5) == "Base Acceleration (m/s^2)") {
      std::getline(dronefile, temp, '\n');
      accels.push_back(std::stof(temp));
    } else if (columns.at(5) == "WeightCapacity (kg)") {
      std::getline(dronefile, temp, '\n');
      weights.push_back(std::stof(temp));
    } else if (columns.at(5) == "Base Battery Capacity (seconds)") {
      std::getline(dronefile, temp, '\n');
      batteries.push_back(std::stof(temp));
    } else if (columns.at(5) == "Model #") {
      std::getline(dronefile, temp, '\n');
      model_names.push_back(temp);
    }
  }

  dronefile.close();

  for (int i = 0; i < model_names.size(); i++) {
    if (model_no_ == model_names[i]) {
      mass_ = masses[i];
      max_speed_ = speeds[i];
      speed_ = max_speed_;
      accel_ = accels[i];
      max_capacity_ = weights[i];
      curr_capacity_ = max_capacity_;
      battery_capacity_ = batteries[i];
      remaining_battery_ = battery_capacity_;
      break;
    }
  }
  /*std::cout << "DRONE.CC set all attributes" << std::endl;
  std::cout << "  model:    " << model_no_ << std::endl;
  std::cout << "  mass:     " << mass_ << std::endl;
  std::cout << "  speed:    " << max_speed_ << std::endl;
  std::cout << "  accel:    " << accel_ << std::endl;
  std::cout << "  capacity: " << max_capacity_ << std::endl;
  std::cout << "  battery:  " << battery_capacity_ << std::endl;*/
}

void csci3081::Drone::Broadcast(const picojson::value val) {
  for (entity_project::EntityObserver* observer : observers_) {
    Drone* currentDrone = this;
    Entity* observedEntity = &(*currentDrone);

    observer->OnEvent(val, (*observedEntity));
  }
}

void csci3081::Drone::EnterIdle() {
  picojson::object o;
  o["type"] = picojson::value("notify");
  o["value"] = picojson::value("idle");
  picojson::value v = picojson::value(o);

  Broadcast(v);
}

picojson::value parseRoute(vector<entity_project::SimpleUMNRoutePoint> path) {
  std::string json = "[";

  for (entity_project::SimpleUMNRoutePoint point : path) {
    json += "[" + std::to_string(point.x) + "," + std::to_string(point.y) +
            "," + std::to_string(point.z) + "],";
  }
  json.pop_back();  // remove last comma
  json += "]";

  picojson::value v;
  std::string err = picojson::parse(v, json);
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }
  return v;
}

void csci3081::Drone::EnterMoving(
    vector<entity_project::SimpleUMNRoutePoint> path) {
  picojson::object o;
  o["type"] = picojson::value("notify");
  o["value"] = picojson::value("moving");
  o["path"] = parseRoute(path);
  picojson::value v = picojson::value(o);

  Broadcast(v);
}
