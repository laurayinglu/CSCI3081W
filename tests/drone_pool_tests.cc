#include <EntityProject/project_settings.h>

#include "../src/drone.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "gtest/gtest.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;
// using entity_project::Drone;

class DronePoolTest : public ::testing::Test {
 protected:
  virtual void SetUp() { system = GetDroneDeliverySystem("default"); }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// TEST_F(DronePoolTest, ReadCSVFile) {
//   DronePool drone_pool;
//   drone_pool.ParseCSVFile();
//   ASSERT_NE(drone_pool.GetIndexes().size(), 0);
// }

// test if initializes properly
TEST_F(DronePoolTest, DroneModelCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Drone* drone = entity->AsType<Drone>();
  ASSERT_NE(drone, nullptr);
  obj["model"] = picojson::value("Q-36-03");
  obj["name"] = picojson::value("drone");
  drone->Set_Attributes(obj);
  ASSERT_EQ(drone->GetMass(), 5.0);
  ASSERT_EQ(drone->GetMaxSpeed(), 40.0);
  ASSERT_EQ(drone->GetMaxCapacity(), 10.0);
  ASSERT_EQ(drone->GetBaseAcceleration(), 5.0);
  ASSERT_EQ(drone->GetBatteryCapacity(), 30.0);

  drone->Set_Remaining_Capacity(3.0);
  ASSERT_EQ(drone->GetRemainingCapacity(), 3.0);
}

// if given model is not in CSV file, the default model "Q-36-01" should be used
TEST_F(DronePoolTest, DefaultModel) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  obj["name"] = picojson::value("drone");
  obj["model"] = picojson::value("X2");
  Entity* entity = system->CreateEntity(obj);
  Drone* drone = entity->AsType<Drone>();
  ASSERT_NE(drone, nullptr);
  drone->Set_Attributes(obj);
  ASSERT_EQ(drone->GetModel(), "Q-36-01");
}

}  // namespace csci3081
