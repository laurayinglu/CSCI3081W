#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include <EntityProject/project_settings.h>
#include <EntityProject/simple_UMN_route_manager.h>

#include <iostream>
#include "../src/vector.h"

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class VectorTest : public ::testing::Test {
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

TEST_F(VectorTest, VectorTests) {
  // test Normalize()
  csci3081::Vector vector;
  entity_project::SimpleUMNRoutePoint point(1, 2, 3);
  vector.Normalize(&point);
  ASSERT_FLOAT_EQ(point.x, 1/sqrt(14));
  ASSERT_FLOAT_EQ(point.y, 2/sqrt(14));
  ASSERT_FLOAT_EQ(point.z, 3/sqrt(14));

  // test Compute_New_Position()
  entity_project::SimpleUMNRoutePoint point2(0, 0, 0);
  entity_project::SimpleUMNRoutePoint new_point =  vector.Compute_New_Position(point, point2, 1, 1);
  ASSERT_FLOAT_EQ(new_point.x, 1/sqrt(14));
  ASSERT_FLOAT_EQ(new_point.y, 2/sqrt(14));
  ASSERT_FLOAT_EQ(new_point.z, 3/sqrt(14));
}

}  // namespace csci3081
