#include <EntityProject/project_settings.h>

#include <iostream>

#include "../src/customer.h"
#include "../src/drone.h"
#include "../src/package.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "gtest/gtest.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class DroneTest : public ::testing::Test {
 protected:
  virtual void SetUp() { system = GetDroneDeliverySystem("default"); }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

/*TEST_F(DroneTest, DroneTests) {
  csci3081::Drone drone;
  float drone_pos [3] = {1,2,3};
  drone.Set_Position(drone_pos);
  drone.Set_Direction(drone_pos);
  //test copy constructor
  csci3081::Drone drone2 = drone;
  std::cout << drone2.GetPosition()[0];
  ASSERT_EQ(drone.GetPosition()[0], drone2.GetPosition()[0]);
  ASSERT_EQ(drone.GetPosition()[1], drone2.GetPosition()[1]);
  ASSERT_EQ(drone.GetPosition()[2], drone2.GetPosition()[2]);

  //test assignment operator
  csci3081::Drone drone3;
  drone3 = drone;
  ASSERT_EQ(drone.GetPosition()[0], drone3.GetPosition()[0]);
  ASSERT_EQ(drone.GetPosition()[1], drone3.GetPosition()[1]);
  ASSERT_EQ(drone.GetPosition()[2], drone3.GetPosition()[2]);
}*/

TEST_F(DroneTest, DroneFunctions) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Drone* drone = entity->AsType<Drone>();
  ASSERT_NE(drone, nullptr);

  // test Set_Attributes() and Set_Details()
  obj["name"] = picojson::value("drone");
  drone->Set_Attributes(obj);
  ASSERT_EQ(drone->GetName(), "drone");
  drone->Set_Details(obj);
  ASSERT_EQ(picojson::value(drone->GetDetails()).serialize(),
            picojson::value(obj).serialize());

  // test Set_Position()
  float pos[3] = {1, 2, 3};
  drone->Set_Position(pos);
  ASSERT_EQ(drone->GetPosition()[0], pos[0]);
  ASSERT_EQ(drone->GetPosition()[1], pos[1]);
  ASSERT_EQ(drone->GetPosition()[2], pos[2]);

  // test Set_Speed()
  float speed = 100;
  drone->Set_Speed(speed);
  ASSERT_EQ(drone->GetCurrentSpeed(), speed);

  // test Set_Delivery_Info()
  Package* package = new Package;
  Customer* customer = new Customer;
  drone->Set_Delivery_Info(package);
  ASSERT_EQ(drone->Get_Assigned_Packagies().at(0), package);

  // test SetRoutes() and UpdateRoute functions
  std::vector<entity_project::SimpleUMNRoutePoint> drone_to_package_route;
  std::vector<entity_project::SimpleUMNRoutePoint> package_to_customer_route;
  drone->SetRoutes(drone_to_package_route, package_to_customer_route);
  for (int i = 0; i < drone_to_package_route.size(); i++) {
    ASSERT_EQ(drone->Get_Drone_To_Package_Route()[i].x,
              drone_to_package_route[i].x);
    ASSERT_EQ(drone->Get_Drone_To_Package_Route()[i].y,
              drone_to_package_route[i].y);
    ASSERT_EQ(drone->Get_Drone_To_Package_Route()[i].z,
              drone_to_package_route[i].z);
  }
  for (int i = 0; i < package_to_customer_route.size(); i++) {
    ASSERT_EQ(drone->Get_Package_To_Customer_Route()[i].x,
              package_to_customer_route[i].x);
    ASSERT_EQ(drone->Get_Package_To_Customer_Route()[i].y,
              package_to_customer_route[i].y);
    ASSERT_EQ(drone->Get_Package_To_Customer_Route()[i].z,
              package_to_customer_route[i].z);
  }

  delete drone;
}
}  // namespace csci3081
