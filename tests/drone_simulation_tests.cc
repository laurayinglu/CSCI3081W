#include <EntityProject/project_settings.h>

#include <iostream>

#include "../src/drone_simulation.h"
#include "gtest/gtest.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class DroneSimulationTest : public ::testing::Test {
 protected:
  virtual void SetUp() { system = GetDroneDeliverySystem("default"); }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(DroneSimulationTest, DroneSimulationFunctions) {
  csci3081::Drone* drone = new Drone();
  csci3081::Customer* cust = new Customer();
  csci3081::Package* package = new Package();
  float drone_pos[3] = {0, 0, 0};
  float cust_pos[3] = {0, 0, 0};
  float package_pos[3] = {200, 200, 200};
  drone->Set_Position(drone_pos);
  cust->Set_Position(cust_pos);
  package->Set_Position(package_pos);

  // test GetTeamName() and default constructor
  DroneSimulation drone_simulation("teamname");
  ASSERT_EQ(drone_simulation.GetTeamName(), "teamname");
  // test CreateEntity(), AddEntity() and GetEntities()
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = drone_simulation.CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  drone_simulation.AddEntity(entity);
  ASSERT_NE(drone_simulation.GetEntities().size(), 0);
  // test ScheduleDelivery() and Update()
  // drone_simulation.ScheduleDelivery(package, cust, obj);
}
}  // namespace csci3081
