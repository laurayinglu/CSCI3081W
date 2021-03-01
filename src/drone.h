#ifndef DRONE_H_
#define DRONE_H_
#include <EntityProject/ANVIL2/drone.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "./customer.h"
#include "./json_parser.h"
#include "./package.h"

using std::vector;

namespace csci3081 {
/**
 * @brief The Drone class that defines drone objects which deliver packages to
 * customers.
 */
class Drone : public entity_project::Drone {  // : public csci3081::DroneFleet {
 public:
  /**
   * Default constructor for a Drone object.
   * @return Reference to a Drone object.
   */
  Drone() { AddType<Drone>(); }
  /**
   * Destructor for dynamically allocated Drone objects.
   * Deletes both inherited and member data.
   */
  ~Drone() {}
  /**
   * Copy constructor that defines how to copy an old Drone object to a
   * new Drone object.
   * @param[in] drone A reference to the drone object to be copied.
   * @return A reference to a new Drone object.
   */
  Drone(const Drone &drone) {
    id_ = drone.id_;
    name_ = drone.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = drone.position_[i];
      direction_[i] = drone.direction_[i];
    }
    radius_ = drone.radius_;
    details_ = drone.details_;
  }

  /**
   * Overloads the assignment operator to allow for assigning one Drone object
   * to another.
   * @param[in] drone A reference to the Drone object to be copied.
   * @return A reference to a new Drone object.
   */
  Drone operator=(const Drone &drone) {
    id_ = drone.id_;
    name_ = drone.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = drone.position_[i];
      direction_[i] = drone.direction_[i];
    }
    radius_ = drone.radius_;
    details_ = drone.details_;
  }
  /**
   * Sets the inherited Entity details for a Drone with a picojson object.
   * @param[in] val A reference to a picojson object.
   */
  void Set_Details(const picojson::object &val) { details_ = val; }
  /**
   * Sets the attributes for a Drone.
   * @param[in] obj A picojson object that will be parsed in order to set the
   * members.
   */
  void Set_Attributes(const picojson::object &obj);
  /**
   * Sets the speed of a Drone.
   * @param[in] speed A float value that is the speed of the drone.
   */

  void Set_Speed(float speed) { speed_ = speed; }

  /**
   * Sets the max speed of a Drone.
   * @param[in] speed A float value that is the max speed of the drone.
   */
  void Set_Max_Speed(float speed) { max_speed_ = speed; }

  std::string GetModel() { return model_no_; }
  float GetMass() { return mass_; }
  float GetBatteryCapacity() { return battery_capacity_; }

  /**
   * Gets the remaining battery of a Drone.
   * @return A float value that is the remaining battery of the drone.
   */
  float GetRemainingBattery() const { return remaining_battery_; }

  /**
   * Gets the speed of a Drone.
   * @return A float value that is the speed of the drone.
   */
  float GetCurrentSpeed() const { return speed_; }

  /**
   * Gets the maximum speed of a Drone.
   * @return A float value that is the maximum speed of the drone.
   */
  float GetMaxSpeed() const { return max_speed_; }

  /**
   * Gets the maximum capacity of a Drone.
   * @return A float value that is the maximum capacity of the drone.
   */
  float GetMaxCapacity() const { return max_capacity_; }

  /**
   * Gets the remaining capacity of a Drone.
   * @return A float value that is the remaining capacity of the drone.
   */
  float GetRemainingCapacity() const { return curr_capacity_; }

  void Set_Remaining_Capacity(float d) { curr_capacity_ = d; }

  /**
   * Gets the base acceleration of a Drone.
   * @return A float value that is the base acceleration of the drone.
   */
  float GetBaseAcceleration() const { return accel_; }
  /**
   * Sets the position of a Drone.
   * @param[in] pos An array of floats that is the current position of the
   * drone.
   */
  void Set_Position(float pos[3]) {
    for (int i = 0; i < 3; i++) {
      position_[i] = pos[i];
    }
  }
  /**
   * Sets the direction of a Drone.
   * @param[in] direction An array of floats that is the current direction of
   * the drone.
   */
  void Set_Direction(float direction[3]) {
    for (int i = 0; i < 3; i++) {
      direction_[i] = direction[i];
    }
  }
  /**
   * Sets the delivery information for a Drone.
   * @param[in] vector of type package A pointer to a Package list.
   * @param[in] vector of type cust A pointer to a Customer list.
   */
  void Set_Delivery_Info(Package *pack) { package_ = pack; }

  void add_Pack(Package *pack) { packageList.push_back(pack); }

  /**
   * Gets the assigned Package for a Drone.
   * @return A pointer to the assigned Package object.
   */
  std::vector<Package *> &Get_Assigned_Packagies() {
    return packageList;
  }

  /**
   * Sets the assigned Package for a Drone.
   * @return A pointer to the assigned Package object.
   */
  void Set_Assigned_Packagies(std::vector<Package *> pl) { packageList = pl; }

  /**
   * Gets the availability for a Drone.
   * @return the availability of the drone.
   */
  bool isAvailable() { return availability; }

  /**
   * Sets the availability for a Drone.
   * @param the new availability of the drone.
   */
  void setAvailability(bool ava) { availability = ava; }

  /**
   * Change the idle mode of the drone
   *
   */
  void changeMode() {
    if (idleMode)
      idleMode = false;
    else
      idleMode = true;
  }

  bool IdleMode() { return idleMode; }

  /**
   * Gets the remaining battery for a Drone.
   *
   * @param[in] duration of time that the battery will be drained.
   */
  void updateBattery(float dt) { remaining_battery_ -= dt; }

  /**
   * Sets the routes inside of a Drone object.
   * @param[in] drone_to_package_route is the route from the drone to the
   * package, as a vector of SimpleUMNRoutePoints
   * @param[in] package_to_customer_route is the route from the package to the
   * customer, as a vector of SimpleUMNRoutePoints.
   */
  void SetRoutes(
      std::vector<entity_project::SimpleUMNRoutePoint> drone_to_package_route,
      std::vector<entity_project::SimpleUMNRoutePoint>
          package_to_customer_route) {
    drone_to_package_route_ = drone_to_package_route;
    package_to_customer_route_ = package_to_customer_route;
  }
  /**
   * Sets the drone_to_package_route. Used to update the route as the drone
   * moves.
   * @param[in] updated_route is the new route from the drone to the package.
   */
  void Update_Drone_To_Package_Route(
      std::vector<entity_project::SimpleUMNRoutePoint> updated_route) {
    drone_to_package_route_ = updated_route;
  }
  /**
   * Sets the package_to_customer_route. Used to update the route as the drone
   * moves.
   * @param[in] updated_route is the new route from the package to the customer.
   */
  void Update_Package_To_Customer_Route(
      std::vector<entity_project::SimpleUMNRoutePoint> updated_route) {
    package_to_customer_route_ = updated_route;
  }
  /**
   * Gets the drone_to_package_route.
   * @return The route from the drone to the package.
   */
  std::vector<entity_project::SimpleUMNRoutePoint>
  Get_Drone_To_Package_Route() {
    return drone_to_package_route_;
  }

  const std::vector<std::string> &GetCurrentRoute() const {
    return current_route_;
  }

  void SetCurrentRoute(std::vector<std::string> current_route) {
    for (int i = 0; i < current_route.size(); i++) {
      // std::cout << current_route.at(i) << std::endl;
      current_route_.push_back(current_route.at(i));
    }
  }

  void PrintCurrentRoute() {
    std::cout << "Printing Current Route" << std::endl;
    for (int i = 0; i < current_route_.size(); i++) {
      std::cout << (current_route_.at(i)) << std::endl;
    }
  }

  /**
   * Gets the package_to_custome_route.
   * @return The route from the package to the customer.
   */
  std::vector<entity_project::SimpleUMNRoutePoint>
  Get_Package_To_Customer_Route() {
    return package_to_customer_route_;
  }

  void Print_Drone_To_Package_Route() {
    for (int i = 0; i < drone_to_package_route_.size(); i++) {
      // std::cout << drone_to_package_route_[i].x << " " <<
      // drone_to_package_route_[i].y << " " << drone_to_package_route_[i].z <<
      // std::endl;
    }
  }

  /**
   * A function that sets a new observer
   * for the drone.
   * @param observer is an EntityObserver
   * */
  void AddObserver(entity_project::EntityObserver *observer) {
    observers_.push_back(observer);
  }
  /**
   * A function that removes an observer
   * for the drone.
   * @param observer is an EntityObserver
   */
  void RemoveObserver(entity_project::EntityObserver *observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
  }

  /**
   * Called when the drone enters an idle state,
   * in order to broadcast that change among observers
   * */
  void EnterIdle();
  /**
   * Called when the drone enters a moving state,
   * in order to broadcast that change among observers
   * */
  void EnterMoving(vector<entity_project::SimpleUMNRoutePoint> path);

  /**
   * Helper to broadcast among observers
   * */
  void Broadcast(const picojson::value val);

  void inc_curr_PickUpPack_Index() { curr_PickUpPack_Index++; }

  int Get_curr_PickUpPack_Index() { return curr_PickUpPack_Index; }

  void inc_curr_DeliverPack_Index() { curr_DeliverPack_Index++; }

  int Get_curr_DeliverPack_Index() { return curr_DeliverPack_Index; }

 protected:
  /// A member variable that is the battery capacity of the drone.
  float battery_capacity_;
  /// A member variable that is the remaining battery of the drone.
  float remaining_battery_;
  /// A member variable that is the current speed of the drone.
  float speed_;
  /// A member variable that is the speed of the drone.
  float max_speed_;
  /// A member variable that is the maximum capacity of the drone.
  float max_capacity_;
  /// A member variable that is the current capacity of the drone.
  float curr_capacity_;
  /// A member variable that is the base acceleration of the drone.
  float accel_;
  /// A member variable that is the Package the Drone is delivering
  csci3081::Package *package_;
  std::vector<csci3081::Package *> packageList;
  /// A member variable that is the Customer that the Drone is delivering to
  csci3081::Customer *customer_;
  /// A member variable that stores the route from the Drone to the Package
  vector<entity_project::SimpleUMNRoutePoint> drone_to_package_route_;
  /// A member variable that stores the route from the Package to the Customer
  vector<entity_project::SimpleUMNRoutePoint> package_to_customer_route_;
  /// vector of observers
  std::vector<entity_project::EntityObserver *> observers_;

  // iteration 2 priority 3 added variables:
  /// A member variable that holds info about the drone.
  std::vector<std::string> drone_info;

  std::vector<std::string> current_route_;
  /// A member variable that holds the model number of the drone.
  std::string model_no_;
  /// A member variable that is the mass of the drone.
  float mass_;
  /// A member variable that indicates if the drone enters idle mode.
  bool idleMode = false;
  /// A member variable that is the availability of the drone.
  bool availability = true;
  /// A member variable that is the current picked up package index.
  int curr_PickUpPack_Index = 0;
  /// A member variable that is the current delivered package index.
  int curr_DeliverPack_Index = 0;
  /// A member variable that holds the variables for each drone type.
  std::vector<std::string> drones_[5];
  /// A member variable that holds the variable names extracted from the CSV
  /// file.
  std::vector<std::string> name_vector;
};
}  // namespace csci3081
#endif
