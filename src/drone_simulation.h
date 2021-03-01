#ifndef DRONE_SIMULATION_H_
#define DRONE_SIMULATION_H_
#include <EntityProject/ANVIL/drone_delivery_system.h>
#include <EntityProject/ANVIL2/drone_delivery_system.h>
#include <EntityProject/entity_console_logger.h>
#include <EntityProject/graph.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "./entity_factory.h"
#include "./json_parser.h"
#include "./movement_system.h"
#include "./scheduling_system.h"

namespace csci3081 {

/** @brief the Drone Simulation class is the implementation of the Drone
 * Delivery System class. This class controls the entire simulation through
 * multiple subsystems.
 */
class DroneSimulation : public entity_project::DroneDeliverySystem,
                        public csci3081::Scheduling_System,
                        public csci3081::Movement_System {
 public:
  /**
   * A default constructor for the DroneSimulation class.
   * @param teamName is a string that contains the name of the team
   * @return a new DroneSimulation object with teamName initialized.
   */
  explicit DroneSimulation(std::string teamName) { teamName_ = teamName; }

  DroneSimulation() { teamName_ = "default name"; }

  /**
   * A destructor for the DroneSimulation class.
   */
  ~DroneSimulation() {
    for (int i = 0; i < entities_.size(); i++) {
      delete entities_[i];
    }
    entities_.clear();
  }


  /**
   * Gets the name of the team.
   * @return Returns a string that is the team name.
   */
  const std::string &GetTeamName() const { return teamName_; }

  /**
   * Method for creating an entity using the factory pattern.
   * @param[in] val A picojson object that includes the necessary information to
   * create the entity.
   * @return Returns a reference to the specific entity.
   */
  entity_project::Entity *CreateEntity(const picojson::object &val) {
    return EntityFactory().MakeEntity(val);
  }

  /**
   * Method for adding an already-created entity to the Drone Delivery System so
   * that it can accomplish goals.
   * @param[in] entity A pointer to an entity.
   */
  void AddEntity(entity_project::Entity *entity) {
    entities_.push_back(entity);
  }

  /**
   * Method for computing the total distance of given route (a vector
   * that has many position point).
   * @param[in] A SimpleUMNRoutePoint vector that represents a route
   * @return Returns the total distance.
   */
  float compute_dist(vector<entity_project::SimpleUMNRoutePoint> route);

  /**
   * Method for getting the drone that is closest to the package. The drone
   * should be available and has enough carry capacity.
   * @param[in] an entities list that has all entities(drone, customer, package)
   * @param[in] a reference to the package needed to be scheduled
   * @return Returns a reference to the chosen drone.
   */
  csci3081::Drone *GetClosestDrone(std::vector<entity_project::Entity *> en,
                                   csci3081::Package *pack);
  /**
   * Method for scheduling a delivery.
   * The method makes use of the UMN Route Manager to create a route between the
   * Drone and the Customer objects.
   * @param[in] package A pointer to a package.
   * @param[in] dest A pointer to a customer.
   * @param[in] details A pointer to a drone.
   */
  void ScheduleDelivery(entity_project::Package *package,
                        entity_project::Customer *dest,
                        const picojson::object &details);

  /**
   * Method that adds the observer to keep track of an entity.
   * Behavior for each specific type of Observer is defined in the
   * observer pattern.
   * @param[in] entity A pointer to an entity object.
   * @param[in] observer A pointer to an observer object.
   */
  void AddObserver(entity_project::Entity *entity,
                   entity_project::EntityObserver *observer);

  /**
   * This method removes a given observer.
   * @param[in] entity A pointer to an entity object.
   * @param[in] observer A pointer to an observer object.
   */
  void RemoveObserver(entity_project::Entity *entity,
                      entity_project::EntityObserver *observer);

  /**
   * This method uses the Entity_System subsystem to keep track of the entities
   * in the Drone Delivery Simulation with a vector of entities.
   * @return Returns a vector of the entities present in the Drone Delivery
   * Simulation.
   */
  const std::vector<entity_project::Entity *> &GetEntities() const {
    return entities_;
  }

  /**
   * This method updates the entities of the Drone Delivery System using a small
   * change in time dt.
   * @param[in] dt A small change in time dt.
   */
  void Update(float dt);

  /**
   * This method get all drones from entities of the Drone Delivery System.
   *
   * @return Returns a drone list that includes all drones in the entities list.
   */
  void SetDroneList();

/**
 * This method sets the graph_ member variable in the Drone Delivery System
 * by taking in an IGraph* from the OSM Graph class.
 * @param[in] graph An IGraph* representing the UMN campus
 * with a number of nodes.
 */
#ifdef ANVIL2
  void SetGraph(const entity_project::IGraph *graph) {
    graph_ = (entity_project::IGraph *)graph;
    return;
  }
#endif

 private:
  ///< A member variable that is the name of the team as a string
  std::string teamName_;

  ///< Vector of pointers to entities currently in Drone Delivery System
  std::vector<entity_project::Entity *> entities_;

  ///< Vector of pointers to Drones currently in entities_ vector
  std::vector<csci3081::Drone *> DroneList;

  ///< An IGraph* that holds information about the UMN campus as a structure of
  ///< nodes.
  entity_project::IGraph *graph_;

  /// represent different weighted graph f= g+h
  /// default is type 0: g = "euclidean" distance h: no heuristic
  /// 1: g = "euclidean" distance h: number of neighbors
  /// 2: g = "euclidean" distance h: euclidean distance
  /// 3: g = "neighbor" distance h: number of neighbors
  /// 4: g = "neighbor" distance h: euclidean
  int graphType;

  int numDrone = 0;
};
}  // namespace csci3081
#endif
