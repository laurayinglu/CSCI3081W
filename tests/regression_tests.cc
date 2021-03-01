#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "EntityProject/ANVIL/customer.h"
#include <EntityProject/project_settings.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>

#include <iostream>
#include "../src/customer.h"
#include "../src/drone.h"
#include "../src/vector.h"
#include "../src/scheduling_system.h"
#include "../src/movement_system.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class RegressionTest : public ::testing::Test {
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

TEST_F(RegressionTest, RegressionTestCreation) {
// test that entities can be successfully created.
  picojson::object obj_cust;
  obj_cust["type"] = picojson::value("customer");
  Entity* entity_cust = system->CreateEntity(obj_cust);
  ASSERT_NE(entity_cust, nullptr);
  Customer* customer = entity_cust->AsType<Customer>();
  ASSERT_NE(customer, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(customer);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
       picojson::value(obj_cust).serialize());

  picojson::object obj_drone;
  obj_drone["type"] = picojson::value("drone");
  Entity* entity_drone = system->CreateEntity(obj_drone);
  ASSERT_NE(entity_drone, nullptr);
  Drone* drone = entity_drone->AsType<Drone>();
  ASSERT_NE(drone, nullptr);
  obj_drone["name"] = picojson::value("drone");
  drone->Set_Attributes(obj_drone);
  ASSERT_EQ(drone->GetName(), "drone");
  drone->Set_Details(obj_drone);
  ASSERT_EQ(picojson::value(drone->GetDetails()).serialize(),
        picojson::value(obj_drone).serialize());

  picojson::object obj_pack;
  obj_pack["type"] = picojson::value("package");
  obj_pack["name"] = picojson::value("package");
  Entity* entity_pack = system->CreateEntity(obj_pack);
  ASSERT_NE(entity_pack, nullptr);
  Package* package = entity_pack->AsType<Package>();
  ASSERT_NE(package, nullptr);
  obj_pack["name"] = picojson::value("package");
  package->Set_Attributes(obj_pack);
  ASSERT_EQ(package->GetName(), "package");
  package->Set_Details(obj_pack);
  ASSERT_EQ(picojson::value(package->GetDetails()).serialize(),
       picojson::value(obj_pack).serialize());
}

TEST_F(RegressionTest, RegressionTestHelper) {
  // ensure that iteration 2 doesn't affect the success of helper functions/classes.
  csci3081::Vector vector;
  entity_project::SimpleUMNRoutePoint point(1, 2, 3);
  vector.Normalize(&point);
  ASSERT_FLOAT_EQ(point.x, 1/sqrt(14));
  ASSERT_FLOAT_EQ(point.y, 2/sqrt(14));
  ASSERT_FLOAT_EQ(point.z, 3/sqrt(14));

  entity_project::SimpleUMNRoutePoint point2(0, 0, 0);
  entity_project::SimpleUMNRoutePoint new_point =  vector.Compute_New_Position(point, point2, 1, 1);
  ASSERT_FLOAT_EQ(new_point.x, 1/sqrt(14));
  ASSERT_FLOAT_EQ(new_point.y, 2/sqrt(14));
  ASSERT_FLOAT_EQ(new_point.z, 3/sqrt(14));
}

TEST_F(RegressionTest, RegressionTestIteration1) {
  // test iteration 2 code for the iteration 1 situation:
  // deliver a single package to a customer, with 1 drone.
  // create a drone, package, and customer
  picojson::object obj_drone;
  obj_drone["type"] = picojson::value("drone");
  obj_drone["name"] = picojson::value("drone");
  picojson::object obj_pack;
  obj_pack["type"] = picojson::value("package");
  obj_pack["name"] = picojson::value("package");
  picojson::object obj_cust;
  obj_cust["type"] = picojson::value("customer");
  obj_cust["name"] = picojson::value("customer");
  Entity* entity_drone = system->CreateEntity(obj_drone);
  Entity* entity_pack = system->CreateEntity(obj_pack);
  Entity* entity_cust = system->CreateEntity(obj_cust);
  system->AddEntity(entity_drone);
  system->AddEntity(entity_pack);
  system->AddEntity(entity_cust);

  Drone* drone = entity_drone->AsType<Drone>();
  vector<csci3081::Drone*> dronelist = {drone};
  Package* package = entity_pack->AsType<Package>();
  Customer* customer = entity_cust->AsType<Customer>();

  float drone_pos[3] = {0, 0, 0};
  drone->Set_Position(drone_pos);
  float pack_pos[3] = {100, 100, 100};
  package->Set_Position(drone_pos);
  float cust_pos[3] = {-200, -200, -200};
  customer->Set_Position(drone_pos);

// setting up a graph
  entity_project::OSMGraphParser parser;
  entity_project::IGraph* graph =
        (entity_project::IGraph*)parser.CreateGraph("data/umn.osm", "data/umn-height.csv");
  std::vector<entity_project::IGraphNode*> nodes = graph->GetNodes();

// testing scheduling system with dijkstras
csci3081::Scheduling_System sched;
picojson::object obj;
sched.Pass_Delivery_Info(drone, package, customer, obj);
ASSERT_EQ(drone->Get_Assigned_Packagies().at(0), package);
ASSERT_EQ(drone->Get_Assigned_Packagies().at(0)->GetCustomer(), customer);
ASSERT_EQ(package->IsDynamic(), true);
sched.Compute_Routes(drone, package, customer, (entity_project::IGraph*) graph);
ASSERT_NE(drone->Get_Drone_To_Package_Route().size(), 0);
ASSERT_NE(drone->Get_Package_To_Customer_Route().size(), 0);

// testing movement system with Package observer
csci3081::Movement_System movement_sys;
// move the drone to the package
while(drone->Get_Drone_To_Package_Route().size() != 0) {
    movement_sys.Update_System(drone, 1, 1, dronelist, graph);
}
ASSERT_LE(abs(drone->GetPosition()[0] - package->GetPosition()[0]), 0.1);
ASSERT_LE(abs(drone->GetPosition()[1] - package->GetPosition()[1]), 0.1);
ASSERT_LE(abs(drone->GetPosition()[2] - package->GetPosition()[2]), 0.1);
// deliver the package to the customer
while(drone->Get_Package_To_Customer_Route().size() != 0) {
  movement_sys.Update_System(drone, 1, 1, dronelist, graph);
}
ASSERT_LE(abs(drone->GetPosition()[0] - customer->GetPosition()[0]) , 0.1);
ASSERT_LE(abs(drone->GetPosition()[1] - customer->GetPosition()[1]), 0.1);
ASSERT_LE(abs(drone->GetPosition()[2] - customer->GetPosition()[2]), 0.1);
}

}  // namespace csci3081
