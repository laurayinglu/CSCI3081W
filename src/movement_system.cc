#include "src/movement_system.h"

#include <iostream>

#include "./customer.h"
#include "./drone.h"
#include "./package.h"
#include "./scheduling_system.h"

// need to take pickedUp packages with drone to move together
void csci3081::Movement_System::Move_Drone(csci3081::Drone* drone, float dt,
                                           float speed) {
  std::vector<entity_project::SimpleUMNRoutePoint> route =
      drone->Get_Drone_To_Package_Route();

  drone->Print_Drone_To_Package_Route();

  entity_project::SimpleUMNRoutePoint current_point =
      entity_project::SimpleUMNRoutePoint(drone->GetPosition()[0],
                                          drone->GetPosition()[1],
                                          drone->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint next_point = route[0];
  if (current_point.DistanceBetween(next_point) == 0) {
    route.erase(route.begin());
    drone->Update_Drone_To_Package_Route(route);
    next_point = route[0];
  }
  /*std::cout << "Next x: " << next_point.x << std::endl;
  std::cout << "Current x: " << current_point.x << std::endl;

  std::cout << "Next y: " << next_point.y << std::endl;
  std::cout << "Current y: " << current_point.y << std::endl;

  std::cout << "Next z: " << next_point.z << std::endl;
  std::cout << "Current z: " << current_point.z << std::endl;*/

  entity_project::SimpleUMNRoutePoint direction_vector =
      entity_project::SimpleUMNRoutePoint(next_point.x - current_point.x,
                                          next_point.y - current_point.y,
                                          next_point.z - current_point.z);

  csci3081::Vector vector;
  vector.Normalize(&direction_vector);

  entity_project::SimpleUMNRoutePoint new_pos =
      vector.Compute_New_Position(direction_vector, current_point, dt, speed);
  // std::cout <<  "New position: " << new_pos.x << std::endl;

  if (current_point.DistanceBetween(new_pos) >=
      current_point.DistanceBetween(next_point)) {
    route.erase(route.begin());
    drone->Update_Drone_To_Package_Route(route);
  }
  float new_arr[3] = {new_pos.x, new_pos.y, new_pos.z};
  drone->Set_Position(new_arr);

  // update pickedUp packages' position
  std::vector<Package*> pl = drone->Get_Assigned_Packagies();
  for (int i = 0; i < pl.size(); i++) {
    if (pl[i]->IsPickedUp()) {
      pl[i]->Set_Position(new_arr);
    }
  }

  /*entity_project::SimpleUMNRoutePoint package_pos =
  entity_project::SimpleUMNRoutePoint(drone->Get_Assigned_Package()->GetPosition()[0],
  drone->Get_Assigned_Package()->GetPosition()[1],
  drone->Get_Assigned_Package()->GetPosition()[2]);
  //check if drone has reached package
  if(drone->GetRadius() >= new_pos.DistanceBetween(package_pos)){
    while(route.size() > 0){
      route.erase(route.begin());
    }
    drone->Update_Drone_To_Package_Route(route);
  }*/

  // check if drone has reached package

  return;
}

// need to take pickedup packages to move together
void csci3081::Movement_System::Move_Package_And_Drone(
    csci3081::Drone* dr, csci3081::Package* package, float dt, float speed) {
  std::vector<entity_project::SimpleUMNRoutePoint> route =
      dr->Get_Package_To_Customer_Route();

  entity_project::SimpleUMNRoutePoint current_point =
      entity_project::SimpleUMNRoutePoint(
          dr->GetPosition()[0], dr->GetPosition()[1], dr->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint next_point = route[0];

  entity_project::SimpleUMNRoutePoint direction_vector =
      entity_project::SimpleUMNRoutePoint(next_point.x - current_point.x,
                                          next_point.y - current_point.y,
                                          next_point.z - current_point.z);
  csci3081::Vector vector;
  vector.Normalize(&direction_vector);

  entity_project::SimpleUMNRoutePoint new_position =
      vector.Compute_New_Position(direction_vector, current_point, dt, speed);

  if (current_point.DistanceBetween(new_position) >=
      current_point.DistanceBetween(next_point)) {
    route.erase(route.begin());
    dr->Update_Package_To_Customer_Route(route);
  }
  float new_arr[3] = {new_position.x, new_position.y, new_position.z};
  dr->Set_Position(new_arr);
  package->Set_Position(new_arr);

  // update pickedUp packages' position
  std::vector<Package*> pl = dr->Get_Assigned_Packagies();
  for (int i = 0; i < pl.size(); i++) {
    if (pl[i]->IsPickedUp()) {
      pl[i]->Set_Position(new_arr);
    }
  }

  // check if drone has reached package
  float customer_pos[3] = {(package->GetCustomer())->GetPosition()[0],
                           (package->GetCustomer())->GetPosition()[1],
                           (package->GetCustomer())->GetPosition()[2]};
  entity_project::SimpleUMNRoutePoint customer_point =
      entity_project::SimpleUMNRoutePoint(customer_pos[0], customer_pos[1],
                                          customer_pos[2]);
  if (dr->GetRadius() >= new_position.DistanceBetween(customer_point)) {
    package->set_Is_Delivered(true);
    route.clear();
    dr->Update_Package_To_Customer_Route(route);
  }
}

// return the drone that is available and have enough capacity
csci3081::Drone* csci3081::Movement_System::getAvailableDrone(
    csci3081::Drone* drone, std::vector<csci3081::Drone*> droneList) {
  csci3081::Drone* best_drone = NULL;
  float shortest_dist = INT_MAX;
  for (int i = 0; i < droneList.size(); i++) {
    float diffx = drone->GetPosition()[0] - droneList[i]->GetPosition()[0];
    float diffy = drone->GetPosition()[1] - droneList[i]->GetPosition()[1];
    float diffz = drone->GetPosition()[2] - droneList[i]->GetPosition()[2];
    float dist = sqrt(pow(diffx, 2) + pow(diffy, 2) + pow(diffz, 2));
    if (droneList[i]->isAvailable() && (dist <= shortest_dist)) {
      if (droneList[i]->GetRemainingCapacity() >=
          (drone->GetMaxCapacity() - drone->GetRemainingCapacity())) {
        best_drone = droneList[i];
        shortest_dist = dist;
      }
    }
  }
  return best_drone;
}


void csci3081::Movement_System::reschedulePackage(
    csci3081::Drone* oldDrone, csci3081::Drone* newDrone,
    entity_project::IGraph* graph, int type) {

  std::vector<Package*> pl = oldDrone->Get_Assigned_Packagies();
  newDrone->Set_Assigned_Packagies(pl);
  //  check if the old drone carries packagies or not
  bool hasPackage = false;
  bool hasAllPackage = true;
  csci3081::Package* oldDroneSpotPack = NULL;
  for (int i = 0; i < pl.size(); i++) {
    if (pl[i]->IsPickedUp()) {
      hasPackage = true;
      oldDroneSpotPack = pl[i];
    }
  }

  Scheduling_System schedule = csci3081::Scheduling_System();
  // if no package carried, then go to grab the first package directly
  if (!hasPackage) {
    schedule.Compute_Paths_To_Packages(newDrone, pl[0], graph, type);
  } else {  // go to oldDrone and pick up packages first
    // reset packagies status: not picked up
    for (int i = 0; i < pl.size(); i++) {
      if (pl[i]->IsPickedUp()) {
        pl[i]->setPickedUp(false);
      }
    }
    schedule.Compute_Paths_To_Packages(newDrone, oldDroneSpotPack, graph, type);
  }

  newDrone->setAvailability(false);
}

void csci3081::Movement_System::Get_Package(
    csci3081::Drone* drone, float dt, float speed,
    std::vector<csci3081::Drone*> droneList, entity_project::IGraph* graph, int type) {
  if (drone->GetRemainingBattery() > 0) {
    Move_Drone(drone, dt, speed);
    drone->updateBattery(dt);
  } else {
    drone->changeMode();
    drone->EnterIdle();
    Drone* newDrone = getAvailableDrone(drone, droneList);

    if (newDrone != NULL) {
      reschedulePackage(drone, newDrone, graph, type);
    } else {
      // std::cout << "No available drones." << std::endl;
    }
  }
}

void csci3081::Movement_System::Deliver_Package(
    csci3081::Drone* drone, float dt, float speed,
    std::vector<csci3081::Drone*> droneList, entity_project::IGraph* graph, int type) {
  // std::cout << "get into deliver package function" << std::endl;
  std::vector<Package*> pl = drone->Get_Assigned_Packagies();
  if (drone->GetRemainingBattery() > 0) {
    Move_Package_And_Drone(drone, pl[0], dt, speed);
    drone->updateBattery(dt);
  } else {
    drone->changeMode();
    drone->EnterIdle();
    Drone* newDrone = getAvailableDrone(drone, droneList);
    if (newDrone != NULL) {
      reschedulePackage(drone, newDrone, graph, type);
    } else {
      // std::cout << "No available drones." << std::endl;
    }
  }
}


bool csci3081::Movement_System::checkAllDelivered(csci3081::Drone* drone) {
  if (drone->Get_Assigned_Packagies().size() == 0) {
    drone->changeMode();
    drone->EnterIdle();
    drone->setAvailability(true);
    return true;
  }

  return false;
}

void csci3081::Movement_System::Update_System(
    csci3081::Drone* drone, float dt, float speed,
    std::vector<csci3081::Drone*> droneList, entity_project::IGraph* graph, int type) {
  // if the drone is not at package's place and not with customer
  if (drone->Get_Drone_To_Package_Route().size() != 0 &&
      drone->Get_Package_To_Customer_Route().size() != 0) {
    Get_Package(drone, dt, speed, droneList, graph, type);

  } else if (drone->Get_Drone_To_Package_Route().size() == 0 &&
             drone->Get_Package_To_Customer_Route().size() != 0) {
    std::vector<Package*> pl = drone->Get_Assigned_Packagies();

    pl[drone->Get_curr_PickUpPack_Index()]->TellBeingCarried();

    Scheduling_System schedule = csci3081::Scheduling_System();

    for (int j = 0; j < pl.size(); j++) {
      if (!pl[j]->IsPickedUp()) {
        schedule.Compute_Paths_To_Packages(drone, pl[j], graph, type);
        drone->inc_curr_PickUpPack_Index();
        return;
      }
    }
    // initial route
    if (drone->Get_curr_DeliverPack_Index() == 0) {
      schedule.Compute_Paths_To_Customers(drone, pl[0], pl[0]->GetCustomer(),
                                          graph, type);
    }

    drone->inc_curr_DeliverPack_Index();

    Deliver_Package(drone, dt, speed, droneList, graph, type);

    // check if packages are delivered
    for (int i = 0; i < drone->Get_Assigned_Packagies().size(); i++) {
      if (drone->Get_Assigned_Packagies()[i]->IsDelivered()) {
        drone->Get_Assigned_Packagies().erase(drone->Get_Assigned_Packagies().begin() + i);
        --i;
      }
    }

    if (checkAllDelivered(drone))
      return;

  } else if (drone->Get_Drone_To_Package_Route().size() == 0 &&
             drone->Get_Package_To_Customer_Route().size() == 0) {
    Scheduling_System schedule = csci3081::Scheduling_System();
    // delete packages that are delivered from its packagelist
    for (int i = 0; i < drone->Get_Assigned_Packagies().size(); i++) {
      if (drone->Get_Assigned_Packagies()[i]->IsDelivered()) {
        drone->Get_Assigned_Packagies().erase(drone->Get_Assigned_Packagies().begin() + i);
        --i;
      }
    }

    // if all packages are delivered
    if (checkAllDelivered(drone)) {
      return;
    }

    schedule.Compute_Paths_To_Customers(drone, drone->Get_Assigned_Packagies()[0],
                                        drone->Get_Assigned_Packagies()[0]->GetCustomer(),
                                        graph, type);
  }
}
