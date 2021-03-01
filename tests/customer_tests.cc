#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "EntityProject/ANVIL/customer.h"
#include <EntityProject/project_settings.h>

#include <iostream>
#include "../src/customer.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class CustomerTest : public ::testing::Test {
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

TEST_F(CustomerTest, CustomerCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Customer* customer = entity->AsType<Customer>();
  ASSERT_NE(customer, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(customer);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
       picojson::value(obj).serialize());
}
TEST_F(CustomerTest, CustomerBigThree) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  obj["name"] = picojson::value("customer");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Customer* customer = entity->AsType<Customer>();
  ASSERT_NE(customer, nullptr);

  // test copy constructor
  Customer customer2 = *customer;
  ASSERT_EQ(customer->GetName(), customer2.GetName());
  ASSERT_EQ(customer->GetPosition()[0], customer2.GetPosition()[0]);
  ASSERT_EQ(customer->GetPosition()[1], customer2.GetPosition()[1]);
  ASSERT_EQ(customer->GetPosition()[2], customer2.GetPosition()[2]);
  ASSERT_EQ(customer->GetDirection()[0], customer2.GetDirection()[0]);
  ASSERT_EQ(customer->GetDirection()[1], customer2.GetDirection()[1]);
  ASSERT_EQ(customer->GetDirection()[2], customer2.GetDirection()[2]);

  // test assignment operator
  Customer customer3;
  customer3 = *customer;
  ASSERT_EQ(customer->GetName(), customer3.GetName());
  ASSERT_EQ(customer->GetPosition()[0], customer3.GetPosition()[0]);
  ASSERT_EQ(customer->GetPosition()[1], customer3.GetPosition()[1]);
  ASSERT_EQ(customer->GetPosition()[2], customer3.GetPosition()[2]);
  ASSERT_EQ(customer->GetDirection()[0], customer3.GetDirection()[0]);
  ASSERT_EQ(customer->GetDirection()[1], customer3.GetDirection()[1]);
  ASSERT_EQ(customer->GetDirection()[2], customer3.GetDirection()[2]);

  // test destructor?
  delete customer;
}
TEST_F(CustomerTest, CustomerSetValuesTest) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Customer* customer = entity->AsType<Customer>();
  ASSERT_NE(customer, nullptr);

  // test Set_Attributes() and Set_Details()
  obj["name"] = picojson::value("customer");
  customer->Set_Attributes(obj);
  ASSERT_EQ(customer->GetName(), "customer");
  customer->Set_Details(obj);
  ASSERT_EQ(picojson::value(customer->GetDetails()).serialize(),
       picojson::value(obj).serialize());
}
TEST_F(CustomerTest, CustomerFunctionsTest) {
  // test Set_Position()
  Customer* customer = new Customer;
  float pos[3]= {1, 2, 3};
  customer->Set_Position(pos);
  ASSERT_EQ(customer->GetPosition()[0], pos[0]);
  ASSERT_EQ(customer->GetPosition()[1], pos[1]);
  ASSERT_EQ(customer->GetPosition()[2], pos[2]);

  // test IsDynamic()
  ASSERT_EQ(customer->IsDynamic(), false);

  delete customer;
}
}  // namespace csci3081
