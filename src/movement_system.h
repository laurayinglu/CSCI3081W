#ifndef MOVEMENT_SYSTEM_H_
#define MOVEMENT_SYSTEM_H_
#include <EntityProject/simple_UMN_route_manager.h>

#include <string>
#include <vector>

#include "./customer.h"
#include "./drone.h"
#include "./json_parser.h"
#include "./package.h"
#include "./vector.h"
#include "EntityProject/graph.h"

namespace csci3081 {
  /**
   * @brief The Movement_System class that implements the movement of Drones and
   * Packages to the Customer.
   */
class Movement_System : public csci3081::Drone {
 public:
  /**
   * The tracking num to track the delivered packages.
   */
  int trackNum = 0;
  /**
   * Checking if all packages of the drone has been delviered
   * 
   * @param[in] The drone needs to be checked
   * @return a bool variable indicates the result.
   */
  bool checkAllDelivered(csci3081::Drone* drone);
  /**
   * The default constructor of the Movement_System class.
   */
  Movement_System() {}
  /**
   * The method that moves the Drone (without a Package).
   * Relies on a route (std::vector of route points) from the Scheduling System
   * and calculates a velocity vector. Either continues towards the current
   * point if the drone is hasn't reached the point yet or finds the next point
   * for the drone to move towards.
   * @param[in] drone A pointer to a Drone.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the Drone (and Package).
   */
  void Move_Drone(csci3081::Drone *drone, float dt, float speed);
  /**
   * The method that moves the Drone and Package once the Drone has picked up
   * the Package Relies on a route (std::vector of rout points) from the
   * Scheduling System and calculates a velocity vector. Either continues
   * towards the current point if the drone is hasn't reached the point yet or
   * finds the next point for the drone to move towards.
   * @param[in] drone A pointer to a Drone.
   * @param[in] package A pointer to a Package.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the Drone (and Package).
   */
  void Move_Package_And_Drone(csci3081::Drone *drone,
                              csci3081::Package *package, float dt,
                              float speed);

  /**
   * The method that gets the closest available drones from
   * the drone list. The returned drone has enough capacity
   * to carry the dropped packages of the old drone.
   * @param[in] drone drone that will drop packages.
   * @param[in] droneList a vector containing all available drones.
   */
  csci3081::Drone *getAvailableDrone(csci3081::Drone *drone,
                                     std::vector<csci3081::Drone *> droneList);

  /**
   * The method that reschdule the dropped packages from old drone
   * to the new drone.
   * @param[in] oldDrone old drone that has battery 0.
   * @param[in] newDrone new drone that will deliver old drone's packages.
   * @param[in] graph a graph that represents the map.
   * @param[in] type that specifies the type of the graph.
   */
  void reschedulePackage(csci3081::Drone *oldDrone, csci3081::Drone *newDrone,
                         entity_project::IGraph *graph, int type);

  /**
   * The method that move the drone to pick up the package.
   * @param[in] drone A pointer to a Drone.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the Drone (and Package).
   * @param[in] droneList a vector containing all available drones.
   * @param[in] graph that represents the map.
   * @param[in] type that specifies the type of the map.
   */
  void Get_Package(csci3081::Drone *drone, float dt, float speed,
                   std::vector<csci3081::Drone *> droneList,
                   entity_project::IGraph *graph, int type);

  /**
   * The method that move the drone to deliver the package.
   * @param[in] drone A pointer to a Drone.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the Drone (and Package).
   * @param[in] droneList a vector containing all available drones.
   * @param[in] graph that represents the map.
   * @param[in] type that specifies the type of the map.
   */
  void Deliver_Package(csci3081::Drone *drone, float dt, float speed,
                       std::vector<csci3081::Drone *> droneList,
                       entity_project::IGraph *graph, int type);

  /**
   * The method that updates the location of the Drone and Package using the
   * methods above. Relies on a route (std::vector of rout points) from the
   * Scheduling System and calculates a velocity vector. Either continues
   * towards the current point if the drone is hasn't reached the point yet or
   * finds the next point for the drone to move towards.
   * @param[in] drone A pointer to a Drone.
   * @param[in] dt A small change in time.
   * @param[in] speed The speed of the Drone (and Package).
   * @param[in] droneList a vector containing all available drones.
   * @param[in] graph that represents the map.
   * @param[in] type that specifies the type of the map.
   */
  void Update_System(csci3081::Drone *drone, float dt, float speed,
                     std::vector<csci3081::Drone *> droneList,
                     entity_project::IGraph *graph, int type);
};
}  // namespace csci3081

#endif
