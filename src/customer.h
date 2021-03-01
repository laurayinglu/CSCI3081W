#ifndef CUSTOMER_H_
#define CUSTOMER_H_
#include <EntityProject/ANVIL2/customer.h>
#include <math.h>

namespace csci3081 {
/**
 * @brief Customer class that defines a Customer object that recieves Packages
 * from Drones.
 */
class Customer : public entity_project::Customer {
 public:
  /**
   * Constructor for a Customer object.
   * @return A pointer to a Customer object.
   */

  Customer() { AddType<Customer>(); }
  /**
   * Destructor for dynamically allocated Customer objects.
   * Deletes both inherited and native member data.
   */
  ~Customer() {}
  /**
   * Copy constructor that defines how to copy an old Customer object to a
   * new Customer object.
   */

  Customer(const Customer &cust) {
    id_ = cust.id_;
    name_ = cust.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = cust.position_[i];
      direction_[i] = cust.direction_[i];
    }
    radius_ = cust.radius_;
    details_ = cust.details_;
  }
  /**
   * Overloads the assignment operator to allow for assigning one Customer
   * object to another.
   *
   */
  Customer operator=(const Customer &cust) {
    id_ = cust.id_;
    name_ = cust.name_;
    for (int i = 0; i < 3; i++) {
      position_[i] = cust.position_[i];
      direction_[i] = cust.direction_[i];
    }
    radius_ = cust.radius_;
    details_ = cust.details_;
    return *this;
  }
  /**
   * Sets the inherited attributes for a Customer via a picojson object.
   * @param[in] obj A reference to a picojson object.
   */
  void Set_Attributes(const picojson::object &object);
  /**
   * Sets the Entity details for a Customer via a picojson object.
   * @param[in] val A reference to a picojson object.
   */
  void Set_Details(const picojson::object &value) { details_ = value; }
  /**
   * Sets the position for a Customer.
   * @param[in] pos A position array of floats.
   */
  void Set_Position(float pos[3]) {
    for (int i = 0; i < 3; i++) {
      position_[i] = pos[i];
    }
  }
  /**
   * A function that returns false because the Customer is not dynamic
   * @return a boolean false value
   */
  bool IsDynamic() const { return false; }
};

}  // namespace csci3081
#endif
