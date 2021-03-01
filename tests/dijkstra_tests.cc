#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/project_settings.h>
#include <EntityProject/simple_UMN_route_manager.h>

#include <iostream>

#include "../src/dijkstra.h"
#include "../src/drone_simulation.h"
#include "gtest/gtest.h"

namespace csci3081 {

using entity_project::Entity;

class DijkstraTest : public ::testing::Test {
 protected:
  virtual void SetUp() { system = GetDroneDeliverySystem("default"); }
  virtual void TearDown() {}

  entity_project::DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(DijkstraTest, DijkstraUnitTests) {
  csci3081::Dijkstra ds;
  entity_project::OSMGraphParser parser;
  const entity_project::IGraph* graph =
      parser.CreateGraph("data/umn.osm", "data/umn-height.csv");
  std::vector<entity_project::IGraphNode*> nodes = graph->GetNodes();
  // test Dijkstra::DistanceBetween()
  entity_project::SimpleUMNRoutePoint start_point(
      nodes.at(0)->GetPosition().at(0), nodes.at(0)->GetPosition().at(1),
      nodes.at(0)->GetPosition().at(2));
  entity_project::SimpleUMNRoutePoint end_point(
      nodes.at(1)->GetPosition().at(0), nodes.at(1)->GetPosition().at(1),
      nodes.at(1)->GetPosition().at(2));
  std::cout << start_point.x << " " << start_point.y << " " << start_point.z;
  ASSERT_FLOAT_EQ(start_point.DistanceBetween(end_point),
                  ds.DistanceBetween(nodes.at(0), nodes.at(1)));

  // test PathToRoute()
  vector<entity_project::IGraphNode*> route =
      ds.ShortestPath(nodes.at(0), nodes.at(1), (entity_project::IGraph*)graph);
  vector<entity_project::SimpleUMNRoutePoint> point_route =
      ds.PathToRoute(route);
  for (int i = 0; i < point_route.size(); i++) {
    ASSERT_FLOAT_EQ(point_route.at(i).x, route.at(i)->GetPosition().at(0));
    ASSERT_FLOAT_EQ(point_route.at(i).y, route.at(i)->GetPosition().at(1));
    ASSERT_FLOAT_EQ(point_route.at(i).z, route.at(i)->GetPosition().at(2));
  }
}

}  // namespace csci3081
