#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>

#include <iostream>
#include "../src/drone.h"
#include "../src/customer.h"
#include "../src/package.h"
#include "../src/movement_system.h"
#include "../src/scheduling_system.h"
#include "EntityProject/osm_graph_parser.h"

namespace csci3081 {


using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class MovementSystemTest : public ::testing::Test {
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

TEST_F(MovementSystemTest, MovementSystemFunctions) {
  csci3081::Drone* drone = new Drone();
  vector<csci3081::Drone*> dronelist = {drone};
  csci3081::Customer* cust = new Customer();
  csci3081::Package* package = new Package();
  float drone_pos[3]= {0, 0, 0};
  float cust_pos[3]= {0, 0, 0};
  float package_pos[3]= {200, 200, 200};
  drone->Set_Position(drone_pos);
  cust->Set_Position(cust_pos);
  package->Set_Position(package_pos);

  Scheduling_System scheduling_system;
  picojson::object obj;
  scheduling_system.Pass_Delivery_Info(drone, package, cust, obj);
  scheduling_system.Compute_Routes_Deprecated(drone, package, cust);

  // test Move_Drone() and Update_System()
  Movement_System movement_system;
  movement_system.Move_Drone(drone, 1, 1);
  ASSERT_NE(drone->GetPosition()[0], drone_pos[0]);
  ASSERT_NE(drone->GetPosition()[1], drone_pos[1]);
  ASSERT_NE(drone->GetPosition()[2], drone_pos[2]);

  drone_pos[0] = drone->GetPosition()[0];
  drone_pos[1] = drone->GetPosition()[1];
  drone_pos[2] = drone->GetPosition()[2];
  entity_project::OSMGraphParser parser;
  entity_project::IGraph* graph =
        (entity_project::IGraph*) parser.CreateGraph("data/umn.osm", "data/umn-height.csv");
  std::vector<entity_project::IGraphNode*> nodes = graph->GetNodes();

  movement_system.Update_System(drone, 1, 1, dronelist, graph);

  ASSERT_NE(drone->GetPosition()[0], drone_pos[0]);
  ASSERT_NE(drone->GetPosition()[1], drone_pos[1]);
  ASSERT_NE(drone->GetPosition()[2], drone_pos[2]);
  // move drone to package
  while (drone->Get_Drone_To_Package_Route().size() != 0) {
    movement_system.Move_Drone(drone, 1, 1);
  }
  // test Move_Package_And_Drone and Update_System()
  movement_system.Move_Package_And_Drone(drone, package, 1, 1);
  ASSERT_NE(package->GetPosition()[0], package_pos[0]);
  ASSERT_NE(package->GetPosition()[1], package_pos[1]);
  ASSERT_NE(package->GetPosition()[2], package_pos[2]);

  package_pos[0] = package->GetPosition()[0];
  package_pos[1] = package->GetPosition()[1];
  package_pos[2] = package->GetPosition()[2];
  movement_system.Update_System(drone, 1, 1, dronelist, graph);
  ASSERT_NE(package->GetPosition()[0], package_pos[0]);
  ASSERT_NE(package->GetPosition()[1], package_pos[1]);
  ASSERT_NE(package->GetPosition()[2], package_pos[2]);
}
}  // namespace csci3081
