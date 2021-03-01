#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>

#include <iostream>
#include "../src/drone.h"
#include "../src/customer.h"
#include "../src/package.h"
#include "../src/scheduling_system.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class SchedulingSystemTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(SchedulingSystemTest, SchedulingSystemFunctions) {
  csci3081::Drone* drone = new Drone();
  csci3081::Customer* cust = new Customer();
  csci3081::Package* package = new Package();
  float drone_pos[3]= {0, 0, 0};
  float cust_pos[3]= {0, 0, 0};
  float package_pos[3]= {200, 200, 200};
  drone->Set_Position(drone_pos);
  cust->Set_Position(cust_pos);
  package->Set_Position(package_pos);

  // test Pass_Delivery_Info() and Compute_Routes()
  Scheduling_System system;
  picojson::object obj;
  system.Pass_Delivery_Info(drone, package, cust, obj);
  ASSERT_NE(drone->Get_Assigned_Packagies().at(0), nullptr);
  ASSERT_NE(drone->Get_Assigned_Packagies().at(0)->GetCustomer(), nullptr);
  system.Compute_Routes_Deprecated(drone, package, cust);
  ASSERT_NE(drone->Get_Drone_To_Package_Route().size(), 0);
  ASSERT_NE(drone->Get_Package_To_Customer_Route().size(), 0);
}
}  // namespace csci3081
