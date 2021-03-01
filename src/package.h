#ifndef PACKAGE_H_
#define PACKAGE_H_
#include <EntityProject/ANVIL2/package.h>
#include <EntityProject/entity_observer.h>
#include <vector>
#include <cmath>
#include <string>
#include "src/customer.h"
#include "src/json_parser.h"

namespace csci3081 {
/**
 * @brief The Package class that represents a Package object which is delivered
 * to a Customer via a Drone.
 */
class Package : public entity_project::Package {
 public:
  /**
   * Package constructor that creates a new Package object.
   * @return Reference to a new Package object.
   */
  Package() { AddType<Package>(); }
  /**
   * Destructor for dynamically allocated Package objects.
   * Deletes both inherited and member data.
   */
  ~Package() {}
  /**
   * Copy constructor that defines how to copy an old package object to a
   * new package object.
   * @param[in] pack A reference to the package object to be copied.
   * @return A reference to a new package object.
   */
  Package(const Package &pack) {
    id_ = pack.id_;
    name_ = pack.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = pack.position_[i];
      direction_[i] = pack.direction_[i];
    }
    radius_ = pack.radius_;
    details_ = pack.details_;
    speed_ = pack.speed_;
    weight_ = pack.weight_;
  }
  /**
   * Overloads the assignment operator to allow for assigning one Package object
   * to another.
   * @param[in] Package A reference to the Package object to be copied.
   * @return pack A reference to a new Package object.
   */
  Package operator=(const Package &pack) {
    id_ = pack.id_;
    name_ = pack.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = pack.position_[i];
      direction_[i] = pack.direction_[i];
    }
    radius_ = pack.radius_;
    details_ = pack.details_;
    speed_ = pack.speed_;
    weight_ = pack.weight_;
  }
  /**
   * Setting the inherited Entity details for a Package via a picojson.
   * @param[in] val A reference to a picojson object.
   */
  void Set_Details(const picojson::object &val) { details_ = val; }
  /**
   * Sets the inherited Entity details for a Package with a picojson object.
   * @param[in] val A reference to a picojson object.
   */
  void Set_Attributes(const picojson::object &obj);
  /**
   * Gets the speed of a Package.
   * @return The speed of the package.
   */
  float GetSpeed() { return speed_; }
  /**
   * Sets the speed of a Package.
   * @param[in] speed A float that is the speed of the package.
   */
  void Set_Speed(float speed) { speed_ = speed; }
  /**
   * Sets the position of a Package.
   * @param[in] pos An array of floats that is the position of the package.
   */
  void Set_Position(float pos[3]) {
    for (int i = 0; i < 3; i++) {
      position_[i] = pos[i];
    }
  }
  /**
   * Sets the direction of a Package.
   * @param[in] direction An array of floats that is the direction of the
   * package.
   */
  void Set_Direction(float direction[3]) {
    for (int i = 0; i < 3; i++) {
      direction_[i] = direction[i];
    }
  }
  /**
   * A function that returns a boolean value denoting whether
   * or not the package is dynamic.
   * @return is_dynamic_ a boolean value.
   * */
  bool IsDynamic() const { return is_dynamic_; }
  /**
   * A function that returns a boolean value denoting whether
   * or not the package has been delivered.
   * @return is_dynamic_ a boolean value.
   * */
  bool IsDelivered() const { return is_delivered_; }
  /**
   * A function that sets a boolean value denoting whether
   * or not the package is dynamic.
   * @param val a boolean value.
   * */
  void set_Is_Dynamic(bool val) {
    is_dynamic_ = val;
    return;
  }
  /**
   * A function that returns a boolean value denoting whether
   * or not the package has been delivered.
   * @param val is a boolean value.
   * */
  void set_Is_Delivered(bool val) {
    if (val && !is_delivered_) {
      OnDelivered();
    }
    is_delivered_ = val;
    return;
  }

  /**
   * Gets the weight of the Package.
   * @return A float value that is the weight of the package.
   */
  float GetWeight() const { return weight_; }

  /**
   * Gets the pciked_up_ status variable for the package.
   * @return A boollean value that signals whether or not the package has been
   * picked up.
   */
  bool GetPickedUp() const { return picked_up_; }

  /**
   * A function that returns a boolean value denoting whether
   * or not the package has been delivered.
   * @param val is a boolean value.
   * */
  void setPickedUp(bool val) {
    picked_up_ = val;
  }
  /**
   * A function that sets a new observer
   * for the package.
   * @param observer is an EntityObserver
   * */
  void AddObserver(entity_project::EntityObserver *observer) {
    observers_.push_back(observer);
  }
  /**
   * A function that removes an observer
   * for the package.
   * @param observer is an EntityObserver
   * */
  void RemoveObserver(entity_project::EntityObserver *observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
  }
  /**
   * A function that broadcasts a message to all observers
   * for the package.
   * @param message is an string that will be part of what each observer
   * receives
   * */
  void Broadcast(std::string message);
  /**
   * Alerts observers of schedule event
   * to be called when package is scheduled
   */
  void OnScheduled() {
    has_scheduled_ = true;
    Broadcast("scheduled");
  }
  /**
   * Alerts observers when the package as been picked up by the drone.
   */
  void TellBeingCarried() {
    if (!picked_up_) {
      Broadcast("en route");
    }
    picked_up_ = true;
  }
  /**
   * Alerts observers when the package has been delivered.
   */
  void OnDelivered() { Broadcast("delivered"); }

  /**
   * Sets the weight of the Package.
   *
   */
  void SetWeight(float newWeight) { weight_ = newWeight; }

  /**
   * Gets the assigned customer of the Package.
   * @return A Customer pointer that is the destination of the package.
   */
  csci3081::Customer *GetCustomer() const { return assignedCustomer; }

  /**
   * Sets the assigned customer of the Package.
   * @param A Customer pointer that is the destination of the package.
   */
  void SetCustomer(csci3081::Customer *customer) {
    assignedCustomer = customer;
  }

  /**
   * Gets the priority of the Package.
   * @return A string that is the priority of the package.
   */
  std::string GetPrioriry() const { return priority; }

  /**
   * Sets the priority of the Package.
   * @param A json object that has the information of priority and
   * minDeliveryTime.
   */
  void SetPriority(const picojson::object &details) {
    if (csci3081::JsonParser::Contains_Key(details, "priority")) {
      priority = csci3081::JsonParser::Get_String(details, "priority");
    }

    if (csci3081::JsonParser::Contains_Key(details, "minDeliveryTime")) {
      minDeliveryTime =
          csci3081::JsonParser::Get_Double(details, "minDeliveryTime");
    }
  }
  /**
   * Check whether the package is picked up or not
   * @return a bool variable indicating the picked up status
   */
  bool IsPickedUp() const { return picked_up_; }
  /**
   * Check whether the package is scheduled or not
   * @return a bool variable indicating the scheduled status
   */
  bool IsScheduled() const { return has_scheduled_; }

 protected:
  /// Speed is a float that is the speed of the package.
  float speed_;
  /// weight_ is the weight of the package.
  float weight_;
  /// priority is the delivery importance of the package.
  std::string priority;
  /// minDeliveryTime is the minimum delivery time of the package.
  float minDeliveryTime;
  /// assignedCustomer is the delivery destination of the package.
  csci3081::Customer *assignedCustomer;
  /// is_dynamic_ is a boolean member variable denoting that the package is
  /// dynamic or not
  bool is_dynamic_ = false;
  /// is_delivered_ is a boolean member variable denoting that the
  /// package has been delivered or not
  bool is_delivered_ = false;
  /// Whether the package has been picked up yet.
  bool picked_up_ = false;
  /// Whether the package has been scheduled yet.
  bool has_scheduled_ = false;
  /// vector of observers
  std::vector<entity_project::EntityObserver *> observers_;
};

}  // namespace csci3081
#endif
