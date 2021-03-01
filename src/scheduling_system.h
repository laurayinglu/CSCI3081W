#ifndef SCHEDULING_SYSTEM_H_
#define SCHEDULING_SYSTEM_H_
#include <EntityProject/simple_UMN_route_manager.h>

#include <vector>

#include "./customer.h"
#include "./dijkstra.h"
#include "./drone.h"
#include "./package.h"

namespace csci3081 {
/**
 * @brief The Scheduling_System class schedules deliveries by passing the
 * Package and Customer information to the Drone.
 */
class Scheduling_System {
 public:
  /**
   * @brief The Scheduling_System default constructor.
   */
  Scheduling_System() {}
  /**
   * The method that passes the Customer and Package information to the Drone.
   * @param[in] drone A reference to a Drone.
   * @param[in] package A reference to a Package.
   * @param[in] destination A reference to a Customer.
   */
  void Pass_Delivery_Info(csci3081::Drone *drone, csci3081::Package *package,
                          csci3081::Customer *destination,
                          const picojson::object &details);

  /**
   * The method that computes the routes between the Drone/Package and
   * Package/Customer and stores them in the drone
   * @param[in] drone A reference to a Drone.
   * @param[in] package A reference to a Package.
   * @param[in] destination A reference to a Customer.
   */
  void Compute_Routes_Deprecated(csci3081::Drone *drone,
                                 csci3081::Package *package,
                                 csci3081::Customer *customer);

  /**
   * The method that computes the routes between the Drone/Package and
   * Package/Customer and stores them in the drone
   * @param[in] drone A reference to a Drone.
   * @param[in] package A reference to a Package.
   * @param[in] destination A reference to a Customer.
   * @param[in] type is an integer flag indicating what kind of algorithm to use.
   */
  void Compute_Routes(csci3081::Drone *drone, csci3081::Package *package,
                      csci3081::Customer *customer,
                      entity_project::IGraph *graph_, int type);

  /**
   * The method that computes the routes between the Drone/Package
   * @param[in] drone A reference to a Drone.
   * @param[in] package A reference to a Package.
   * @param[in] graph is a graph of nodes that is used to create the shortest path
   * @param[in] type is an integer flag indicating what kind of algorithm to use.
   */
  void Compute_Paths_To_Packages(csci3081::Drone *dr, csci3081::Package *pack,
                                 entity_project::IGraph *graph, int type);


  /**
   * The method that computes the routes between the Drone/Customer
   * @param[in] drone A reference to a Drone.
   * @param[in] package A reference to a Package.
   * @param[in] graph is a graph of nodes that is used to create the shortest path
   * @param[in] type is an integer flag indicating what kind of algorithm to use.
   */
  void Compute_Paths_To_Customers(csci3081::Drone *dr, csci3081::Package *pack,
                                  csci3081::Customer *cust,
                                  entity_project::IGraph *graph, int type);
};
}  // namespace csci3081

#endif
