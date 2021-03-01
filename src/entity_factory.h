#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_

#include <EntityProject/entity.h>
#include <string>
#include "./customer.h"
#include "./drone.h"
#include "./json_parser.h"
#include "./package.h"

namespace csci3081 {
/**
 * @brief This class makes entities for the Drone Simulation class. It is part
 * of the factory pattern.
 */
class EntityFactory {
 public:
  /**
   * @brief Constructor to instantiate the EntityFactory class.
   */
  EntityFactory() {}
  /**
   * Method that makes the entity based off of the information in the obj
   * paramter.
   * @param[in] obj A picojson object that contains the details for tje entity.
   */
  entity_project::Entity *MakeEntity(const picojson::object &obj);
};
}  // namespace csci3081

#endif
