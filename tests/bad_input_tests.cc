#include <EntityProject/project_settings.h>

#include <iostream>

#include "../src/customer.h"
#include "../src/drone_simulation.h"
#include "../src/json_parser.h"
#include "../src/package.h"
#include "EntityProject/ANVIL/customer.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "gtest/gtest.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class BadInputTest : public ::testing::Test {
 protected:
  virtual void SetUp() { system = GetDroneDeliverySystem("default"); }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(BadInputTest, UpdateIsZero) {
  csci3081::Drone* drone = new csci3081::Drone();
  float drone_pos[3] = {1, 2, 3};
  drone->Set_Position(drone_pos);
  csci3081::DroneSimulation* sim = new csci3081::DroneSimulation();
  sim->AddEntity(drone);
  sim->Update(0);
  ASSERT_NE(drone->GetPosition()[0], drone_pos[0]);
  ASSERT_NE(drone->GetPosition()[1], drone_pos[1]);
  ASSERT_NE(drone->GetPosition()[2], drone_pos[2]);
}
TEST_F(BadInputTest, NoObserverAdded) {
  csci3081::DroneSimulation* sim = new csci3081::DroneSimulation();
  picojson::object obj1, obj2, obj3, obj4;
  obj1["type"] = picojson::value("package");
  Entity* entity1 = system->CreateEntity(obj1);
  system->AddEntity(entity1);
  ASSERT_EQ(
      system->GetEntities().at(0)->AsType<csci3081::Package>()->IsPickedUp(),
      true);
}
TEST_F(BadInputTest, ScheduleWrongCustomer) {}
TEST_F(BadInputTest, EntityNotUnique) {
  csci3081::DroneSimulation* sim = new csci3081::DroneSimulation();
  picojson::object obj1, obj2, obj3, obj4;
  obj1["type"] = picojson::value("drone");
  obj1["id"] = picojson::value("1");
  Entity* entity1 = system->CreateEntity(obj1);
  system->AddEntity(entity1);
  obj2["type"] = picojson::value("customer");
  obj2["id"] = picojson::value("2");
  Entity* entity2 = system->CreateEntity(obj2);
  system->AddEntity(entity2);
  obj3["type"] = picojson::value("package");
  obj3["id"] = picojson::value("3");
  Entity* entity3 = system->CreateEntity(obj3);
  system->AddEntity(entity3);
  obj4["type"] = picojson::value("package");
  obj4["id"] = picojson::value("3");
  Entity* entity4 = system->CreateEntity(obj4);
  std::vector<Entity*> entities = system->GetEntities();
  for (int i = 0; i < entities.size(); i++) {
    ASSERT_NE(entity4->GetId(), entities.at(i)->GetId());
  }
}
TEST_F(BadInputTest, EntityRadiusIsWrong) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  obj["radius"] = picojson::value("1");
  Entity* entity = system->CreateEntity(obj);
  Drone* drone = entity->AsType<Drone>();
  system->AddEntity(drone);
  ASSERT_EQ(drone->GetRadius(), 1);
}

}  // namespace csci3081
