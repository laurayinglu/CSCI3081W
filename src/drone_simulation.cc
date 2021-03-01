#include "src/drone_simulation.h"

#include <math.h>

float csci3081::DroneSimulation::compute_dist(
      vector<entity_project::SimpleUMNRoutePoint> route) {
  float total_dist = 0;
  for (int i = 0; i < route.size()-1; i++) {
    float diffx = route[i].x - route[i+1].x;
    float diffy = route[i].y - route[i+1].y;
    float diffz = route[i].z - route[i+1].z;
    float dist = sqrt(pow(diffx, 2) + pow(diffy, 2) + pow(diffz, 2));
    total_dist += dist;
  }

  return total_dist;
}

csci3081::Drone* csci3081::DroneSimulation::GetClosestDrone(
    std::vector<entity_project::Entity*> en, csci3081::Package* pack) {

  csci3081::Drone* close_Available_Drone = NULL;
  float shortest_dist = INT_MAX;
  float dist;
  for (int i = 0; i < entities_.size(); i++) {
    Drone* dr = entities_[i]->AsType<Drone>();
    if (dr) {
      // std::cout << dr->GetRemainingCapacity() << std::endl;
      // std::cout << pack->GetWeight() << std::endl;
    }
    if (dr && dr->isAvailable() &&
        (dr->GetRemainingCapacity() >= pack->GetWeight())) {
      csci3081::Scheduling_System().Compute_Paths_To_Packages(
                                          dr, pack, graph_, graphType);

      vector<entity_project::SimpleUMNRoutePoint> route =
              dr->Get_Drone_To_Package_Route();
      dist = compute_dist(route);

      if (dist <= shortest_dist) {
        shortest_dist = dist;
        close_Available_Drone = dr;
      }
    }
  }

  // update chosen drone's capacity
  if (close_Available_Drone != NULL) {
    float d = close_Available_Drone->GetRemainingCapacity() - pack->GetWeight();
    close_Available_Drone->Set_Remaining_Capacity(d);
  }

  return close_Available_Drone;
}

/**
 * Method for scheduling a delivery.
 * The method makes use of the UMN Route Manager to create a route between the
 * Drone and the Customer objects.
 * @param[in] package A pointer to a package.
 * @param[in] dest A pointer to a customer.
 * @param[in] details A pointer to a drone.
 */
void csci3081::DroneSimulation::ScheduleDelivery(
    entity_project::Package* package, entity_project::Customer* dest,
    const picojson::object& details) {

  SetDroneList();
  
  // get the drone closest to the package
  csci3081::Drone* arrangedDrone =
      GetClosestDrone(entities_, package->AsType<csci3081::Package>());

  if (arrangedDrone) {
    numDrone++;
    std::cout << "num drone" << numDrone << std::endl;
    if (!package->AsType<csci3081::Package>()->IsScheduled()) {
      package->AsType<csci3081::Package>()->OnScheduled();
      Scheduling_System schedule = csci3081::Scheduling_System();

      schedule.Pass_Delivery_Info(arrangedDrone,
                                  package->AsType<csci3081::Package>(),
                                  dest->AsType<csci3081::Customer>(), details);

      std::vector<Package*> pl = arrangedDrone->Get_Assigned_Packagies();
      
      if (pl.size() > 0) {
        for (int i = 0; i <= 4; i++) {
          graphType = i;
          // std::cout << "Starting graph " << i << std::endl;
          //if(numDrone == 1)
          schedule.Compute_Routes(arrangedDrone, pl[0], pl[0]->GetCustomer(), graph_, i);
          //else
            //schedule.Compute_Routes(arrangedDrone, pl[0], pl[0]->GetCustomer(), graph_, 3);
          // std::cout << "Finished graph " << i << std::endl;
        }
        // schedule.Compute_Routes(arrangedDrone, pl[0], pl[0]->GetCustomer(), graph_, 4);
      }
    }
  } else {
    //  std::cout << "no available drones" << std::endl;
  }
}

/**
 * This method updates the entities of the Drone Delivery System using a small
 * change in time dt.
 * @param[in] dt A small change in time dt.
 */
void csci3081::DroneSimulation::Update(float dt) {
  // std::cerr << "enter "<< std::endl;
  if (entities_.size() < 3) {
    // std::cout << "not enough entities" << std::endl;
    // return;
  } else {
    for (int i = 0; i < DroneList.size(); i++) {
      Drone* dr = DroneList[i];
      std::vector<Package*> pl = dr->Get_Assigned_Packagies();

      if (dr->IdleMode()) {
        if ((pl.size()) != 0) {
          dr->changeMode();  // reactivate the drone
        }
      }

      if (!dr->IdleMode()) {
        if ((dr->Get_Assigned_Packagies().size()) != 0) {
          dr->setAvailability(false);
          float speed = dr->GetCurrentSpeed();
          csci3081::Movement_System().Update_System(dr, dt, speed, DroneList,
                                                    graph_, graphType);
        } else {
          continue;
        }
      } else {
        break;
      }
    }

    // delete packages that are delivered
    for (int i = 0; i < entities_.size(); i++) {
      Package* pack = entities_[i]->AsType<csci3081::Package>();

      if (pack && pack->IsDelivered()) {
        delete pack;
        entities_.erase(std::find(entities_.begin(), entities_.end(), pack));
      }
    }
  }
}

void csci3081::DroneSimulation::RemoveObserver(
    entity_project::Entity* entity, entity_project::EntityObserver* observer) {
  csci3081::Package* ifPackage = entity->AsType<csci3081::Package>();
  if (ifPackage) {
    ifPackage->RemoveObserver(observer);
  }
  csci3081::Drone* ifDrone = entity->AsType<csci3081::Drone>();
  if (ifDrone) {
    ifDrone->RemoveObserver(observer);
  }
}

void csci3081::DroneSimulation::AddObserver(
    entity_project::Entity* entity, entity_project::EntityObserver* observer) {
  csci3081::Package* ifPackage = entity->AsType<csci3081::Package>();
  if (ifPackage) {
    ifPackage->AddObserver(observer);
  }
  csci3081::Drone* ifDrone = entity->AsType<csci3081::Drone>();
  if (ifDrone) {
    ifDrone->AddObserver(observer);
  }
}

/**
 * This method get the drones from entities_ and save them in
 * the droneList.
 */
void csci3081::DroneSimulation::SetDroneList() {
  // std::vector<Drone*> droneList;
  DroneList.clear();
  for (int i = 0; i < entities_.size(); i++) {
    csci3081::Drone* myDrone = entities_[i]->AsType<Drone>();
    if (myDrone) {
      DroneList.push_back(myDrone);
    }
  }
}
