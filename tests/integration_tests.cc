#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <EntityProject/osm_graph_parser.h>

#include <iostream>
#include "../src/dijkstra.h"
#include "../src/drone_simulation.h"
#include "../src/package.h"

namespace csci3081 {

using entity_project::Entity;

class IntegrationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
  }
  virtual void TearDown() {}

  entity_project::DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(IntegrationTest, IntegrationTests) {
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

// test DroneDeliverySystem with EntityFactory and JSON Parser
  Drone* drone = entity_drone->AsType<Drone>();
  std::vector<csci3081::Drone*> dronelist = {drone};
  ASSERT_NE(drone, nullptr);
  Package* package = entity_pack->AsType<Package>();
  ASSERT_NE(package, nullptr);
  Customer* customer = entity_cust->AsType<Customer>();
  ASSERT_NE(customer, nullptr);

  float drone_pos[3] = {0, 0, 0};
  drone->Set_Position(drone_pos);
  float pack_pos[3] = {100, 100, 100};
  package->Set_Position(drone_pos);
  float cust_pos[3] = {-200, -200, -200};
  customer->Set_Position(drone_pos);

// setting up graph
  entity_project::OSMGraphParser parser;
  entity_project::IGraph* graph =
      (entity_project::IGraph*)parser.CreateGraph("data/umn.osm", "data/umn-height.csv");
  std::vector<entity_project::IGraphNode*> nodes = graph->GetNodes();

// testing scheduling system with dijkstras
csci3081::Scheduling_System sched;
const picojson::object obj;
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
    movement_sys.Update_System(drone, 0.05, 0.1, dronelist, graph);
}
ASSERT_LE(abs(drone->GetPosition()[0] - package->GetPosition()[0]), 0.1);
ASSERT_LE(abs(drone->GetPosition()[1] - package->GetPosition()[1]), 0.1);
ASSERT_LE(abs(drone->GetPosition()[2] - package->GetPosition()[2]), 0.1);

// test if the observer will broadcast that the package has been picked up
movement_sys.Update_System(drone, 0.05, 0.1, dronelist, graph);
ASSERT_EQ(package->GetPickedUp(), true);
}

}  // namespace csci3081
