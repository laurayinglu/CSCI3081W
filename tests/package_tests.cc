#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include <EntityProject/project_settings.h>
#include "src/drone_simulation.h"

#include <iostream>
#include "../src/package.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class PackageTest : public ::testing::Test {
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

TEST_F(PackageTest, PackageCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("package");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Package* package = entity->AsType<Package>();
  ASSERT_NE(package, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(package);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
       picojson::value(obj).serialize());
}
/*TEST_F(PackageTest, PackageBigThree){
  Package* package = new Package();
  float package_pos [3] = {1,2,3};
  package->Set_Position(package_pos);
  //test copy constructor
  Package package2 = *package;
  ASSERT_EQ(package->GetPosition()[0], package2.GetPosition()[0]);
  ASSERT_EQ(package->GetPosition()[1], package2.GetPosition()[1]);
  ASSERT_EQ(package->GetPosition()[2], package2.GetPosition()[2]);

  
  //test assignment operator
  Package package3;
  package3 = *package;
  ASSERT_EQ(package->GetName(), package3.GetName());
  ASSERT_EQ(package->GetPosition()[0], package3.GetPosition()[0]);
  ASSERT_EQ(package->GetPosition()[1], package3.GetPosition()[1]);
  ASSERT_EQ(package->GetPosition()[2], package3.GetPosition()[2]);

  //test destructor?
  delete package;

}*/
TEST_F(PackageTest, PackageSetValuesTest) {
  picojson::object obj;
  obj["type"] = picojson::value("package");
  obj["name"] = picojson::value("package");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Package* package = entity->AsType<Package>();
  ASSERT_NE(package, nullptr);

  // test Set_Attributes() and Set_Details()
  obj["name"] = picojson::value("package");
  package->Set_Attributes(obj);
  ASSERT_EQ(package->GetName(), "package");
  package->Set_Details(obj);
  ASSERT_EQ(picojson::value(package->GetDetails()).serialize(),
       picojson::value(obj).serialize());
}
TEST_F(PackageTest, PackageFunctionsTest) {
  // test Set_Position()
  Package* package = new Package();
  float pos[3]= {1, 2, 3};
  package->Set_Position(pos);
  ASSERT_EQ(package->GetPosition()[0], pos[0]);
  ASSERT_EQ(package->GetPosition()[1], pos[1]);
  ASSERT_EQ(package->GetPosition()[2], pos[2]);

  // test Set_Is_Dynamic()
  package->set_Is_Dynamic(true);
  ASSERT_EQ(package->IsDynamic(), true);

  delete package;
}

/*TEST_F(PackageTest, PackageUnitTest) {
  csci3081::DroneSimulation sim;
  picojson::object obj;
  obj["type"] = picojson::value("package");
  obj["name"] = picojson::value("package");
  Entity* entity = system->CreateEntity(obj);
  Package* package = entity->AsType<Package>();
  sim.AddObserver(package, package.)

}*/
}  // namespace csci3081
