#include <EntityProject/simple_UMN_route_manager.h>
#include <math.h>
#include <iostream>
#include <vector>
#ifndef VECTOR_H_
#define VECTOR_H_

namespace csci3081 {
/**
 * @brief The Vector class is a helper class that performs vector operations.
 */
class Vector {
 public:
  /**
   * Normalize normalize the vector vec and updates its x, y, and z positions
   * accordingly.
   * @param[in] vec A vector that is going to be normalized.
   */
  void Normalize(entity_project::SimpleUMNRoutePoint* vec) {
    // std::cout << vec.x << std::endl;
    float l2_norm = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
    // std::cout << l2_norm << std::endl;
    vec->x /= l2_norm;
    vec->y /= l2_norm;
    vec->z /= l2_norm;
    /*std::cout <<  "x after norm" << vec.x << std::endl;
std::cout <<  "y after norm" << vec.y << std::endl;
std::cout <<  "z after norm" << vec.z << std::endl;*/
  }
  /**
   * Compute_New_Position takes in a normalized vector, change in time and speed
   * to compute the new position of a point.
   * @param[in] normed_vec A normalized direction vector.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the object which will have a new position.
   */
  entity_project::SimpleUMNRoutePoint Compute_New_Position(
      entity_project::SimpleUMNRoutePoint normed_vec,
      entity_project::SimpleUMNRoutePoint current, float dt, float speed) {
    entity_project::SimpleUMNRoutePoint new_position =
        entity_project::SimpleUMNRoutePoint(0, 0, 0);
    new_position.x = current.x + normed_vec.x * speed * dt;
    new_position.y = current.y + normed_vec.y * speed * dt;
    new_position.z = current.z + normed_vec.z * speed * dt;
    // std::cout <<  "new position x" << new_position.x << std::endl;
    return new_position;
  }
};

}  // namespace csci3081
#endif
