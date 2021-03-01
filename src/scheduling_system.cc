#include "src/scheduling_system.h"

#include <EntityProject/graph.h>
#include <EntityProject/simple_UMN_route_manager.h>

#include <iostream>
#include <string>
#include <time.h>

// set delivery info for package and drone
void csci3081::Scheduling_System::Pass_Delivery_Info(csci3081::Drone* drone,
                  csci3081::Package* package, csci3081::Customer* destination,
                                const picojson::object& details) {
  package->SetCustomer(destination);
  package->SetPriority(details);
  package->set_Is_Dynamic(true);

  // drone->Set_Pack(package);
  drone->add_Pack(package);

  //   std::cout << "Delivery scheduled" << std::endl;
  //     std::cout << "Drone position: [";
  //      for(int i = 0; i < 3; i++){
  //          std::cout << drone->GetPosition()[i] << " ";
  //      }
  //      std::cout << "]" << std::endl;
  //      std::cout << "Package position: [";
  //      for(int i = 0; i < 3; i++){
  //          std::cout << package->GetPosition()[i] << " ";
  //      }
  //      std::cout << "]" << std::endl;
  //      std::cout << "Customer position: [";
  //      for(int i = 0; i < 3; i++){
  //          std::cout << destination->GetPosition()[i] << " ";
  //      }
  //      std::cout << "]" << std::endl;
}

void csci3081::Scheduling_System::Compute_Routes_Deprecated(
    csci3081::Drone* drone, csci3081::Package* package,
    csci3081::Customer* customer) {
  entity_project::SimpleUMNRoutePoint drone_position =
      entity_project::SimpleUMNRoutePoint(drone->GetPosition()[0],
                                          drone->GetPosition()[1],
                                          drone->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint package_position =
      entity_project::SimpleUMNRoutePoint(package->GetPosition()[0],
                                          package->GetPosition()[1],
                                          package->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint customer_position =
      entity_project::SimpleUMNRoutePoint(customer->GetPosition()[0],
                                          customer->GetPosition()[1],
                                          customer->GetPosition()[2]);

  entity_project::SimpleUMNRouteManager rm;
  vector<entity_project::SimpleUMNRoutePoint> pos_to_pack_rt =
      rm.GetRoute(drone_position, package_position);
  vector<entity_project::SimpleUMNRoutePoint> pack_to_cust_rt =
      rm.GetRoute(package_position, customer_position);
  drone->SetRoutes(pos_to_pack_rt, pack_to_cust_rt);
  // If we need the full route, use
  // vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
  // below, but mind that it doesn't screw up the routes already set
  // (I'm not sure if its reference or copy)
  drone->EnterMoving(pos_to_pack_rt);
  // std::cout << route_manager.GetRoute(drone_position,
  // package_position).size() << std::endl;
  // drone->Print_Drone_To_Package_Route();
  // std::cout << package->GetPosition()[0] << " " << package->GetPosition()[1]
  // << package->GetPosition()[2] << std::endl;
  // std::cout << "Route info passed to Drone" << std::endl;
}

void csci3081::Scheduling_System::Compute_Routes(csci3081::Drone* dr,
    csci3081::Package* pack, csci3081::Customer* cust,
    entity_project::IGraph* graph, int type) {

  // begin timing
  std::clock_t start = std::clock();

  // std::cout << "Within compute routes" << std::endl;
  entity_project::SimpleUMNRoutePoint dr_pos =
      entity_project::SimpleUMNRoutePoint(
          dr->GetPosition()[0], dr->GetPosition()[1], dr->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint package_pos =
      entity_project::SimpleUMNRoutePoint(pack->GetPosition()[0],
                                          pack->GetPosition()[1],
                                          pack->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint cust_pos =
      entity_project::SimpleUMNRoutePoint(cust->GetPosition()[0],
                                          cust->GetPosition()[1],
                                          cust->GetPosition()[2]);

  csci3081::Dijkstra ds;

  entity_project::IGraphNode* dr_node =
      ds.FindNodeFromEntityPosition(dr_pos, graph);
  entity_project::IGraphNode* pack_node =
      ds.FindNodeFromEntityPosition(package_pos, graph);
  entity_project::IGraphNode* cust_node =
      ds.FindNodeFromEntityPosition(cust_pos, graph);

  std::vector<entity_project::IGraphNode*> drone_to_package_nodes =
      ds.ShortestPath(dr_node, pack_node, graph, type);
  std::vector<entity_project::IGraphNode*> package_to_customer_nodes;

  if (package_pos.DistanceBetween(cust_pos) >= 0.01) {
    package_to_customer_nodes = ds.ShortestPath(pack_node, cust_node, graph, type);
  }

  std::vector<entity_project::SimpleUMNRoutePoint> drone_to_pack =
      ds.PathToRoute(drone_to_package_nodes);
  drone_to_pack.push_back(package_pos);

  std::vector<std::string> current_route;
  // std::cout << "CURRENT ROUTE" << std::endl;
  for (int i = 0; i < drone_to_package_nodes.size(); i++) {
    current_route.push_back(drone_to_package_nodes[i]->GetName());
    // std::cout << drone_to_package_nodes[i]->GetPosition().at(0) << " " <<
    // drone_to_package_nodes[i]->GetPosition().at(1) << " " <<
    // drone_to_package_nodes[i]->GetPosition().at(2) << std::endl;
  }
  if (package_pos.DistanceBetween(cust_pos) >= 0.01) {
    current_route.pop_back();
    for (int i = 0; i < package_to_customer_nodes.size(); i++) {
      current_route.push_back(package_to_customer_nodes[i]->GetName());
      // std::cout << package_to_customer_nodes[i]->GetPosition().at(0) << " "
      // << package_to_customer_nodes[i]->GetPosition().at(1) << " " <<
      // package_to_customer_nodes[i]->GetPosition().at(2) << std::endl;
    }
  }
  /*std::cout << "Current Route" << std::endl;
  for(int i = 0; i < current_route.size(); i++){
      std::cout << current_route.at(i) << std::endl;
  }*/
  dr->SetCurrentRoute(current_route);
  std::vector<entity_project::SimpleUMNRoutePoint> pack_to_cust =
      ds.PathToRoute(package_to_customer_nodes);
  pack_to_cust.push_back(cust_pos);

  // dr->SetCurrentRoute(current_route);
  dr->SetRoutes(drone_to_pack, pack_to_cust);

  std::cout << "Graph " << type << " visited " << ds.totalnodes << " nodes." << std::endl;

  // finish timing
  std::clock_t end = std::clock();
  double elapsed = (static_cast<double>(end - start)) / CLOCKS_PER_SEC;
  std::cout << "Graph " << type << " time to execute: " << elapsed << " seconds" << std::endl;

  float result = 100*(static_cast<float> (ds.totalnodes) / static_cast<float> (ds.nodes_in_graph));
  std::cout << "Graph " << type << " visited " << result << "% of the graph" << std::endl;

  float total_dist = 0;
  for (int i = 0; i < drone_to_package_nodes.size()-1; i++) {
    total_dist += ds.DistanceBetween(drone_to_package_nodes.at(i), drone_to_package_nodes.at(i+1));
  }
  for (int i = 0; i < package_to_customer_nodes.size()-1; i++) {
    total_dist += ds.DistanceBetween(package_to_customer_nodes.at(i),
                                package_to_customer_nodes.at(i+1));
  }
  std::cout << "path length: " << total_dist << std::endl;

  // float dist2 = 0;
  // for (int i = 0; i < drone_to_pack.size()-1; i++) {
  //   float diffx = drone_to_pack[i].x - drone_to_pack[i+1].x;
  //   float diffy = drone_to_pack[i].y - drone_to_pack[i+1].y;
  //   float diffz = drone_to_pack[i].z - drone_to_pack[i+1].z;
  //   float dist = sqrt(pow(diffx, 2) + pow(diffy, 2) + pow(diffz, 2));
  //   dist2 += dist;
  // }
  // for (int i = 0; i < pack_to_cust.size()-1; i++) {
  //   float diffx = pack_to_cust[i].x - pack_to_cust[i+1].x;
  //   float diffy = pack_to_cust[i].y - pack_to_cust[i+1].y;
  //   float diffz = pack_to_cust[i].z - pack_to_cust[i+1].z;
  //   float dist = sqrt(pow(diffx, 2) + pow(diffy, 2) + pow(diffz, 2));
  //   dist2 += dist;
  // }
  // std::cout << "dist2: " << dist2 << std::endl;

  std::cout << " --------- " << std::endl;
}

void csci3081::Scheduling_System::Compute_Paths_To_Packages(
    csci3081::Drone* dr, csci3081::Package* pack,
    entity_project::IGraph* graph, int type) {
  // std::cout << "Within compute paths to packages" << std::endl;

  entity_project::SimpleUMNRoutePoint dr_pos =
      entity_project::SimpleUMNRoutePoint(
          dr->GetPosition()[0], dr->GetPosition()[1], dr->GetPosition()[2]);

  entity_project::SimpleUMNRoutePoint package_pos =
      entity_project::SimpleUMNRoutePoint(pack->GetPosition()[0],
                                          pack->GetPosition()[1],
                                          pack->GetPosition()[2]);

  csci3081::Dijkstra ds;

  entity_project::IGraphNode* dr_node =
      ds.FindNodeFromEntityPosition(dr_pos, graph);
  entity_project::IGraphNode* pack_node =
      ds.FindNodeFromEntityPosition(package_pos, graph);

  vector<entity_project::IGraphNode*> drone_to_package_nodes =
      ds.ShortestPath(dr_node, pack_node, graph, type);
  std::vector<std::string> current_route;
  for (int i = 0; i < drone_to_package_nodes.size(); i++) {
    current_route.push_back(drone_to_package_nodes[i]->GetName());
    // std::cout << drone_to_package_nodes[i]->GetPosition().at(0) << " " <<
    // drone_to_package_nodes[i]->GetPosition().at(1) << " " <<
    // drone_to_package_nodes[i]->GetPosition().at(2) << std::endl;
  }
  // dr->SetCurrentRoute(current_route);
  // std::cout << "shortest paths created" << std::endl;
  std::vector<entity_project::SimpleUMNRoutePoint> drone_to_pack =
      ds.PathToRoute(drone_to_package_nodes);

  drone_to_pack.push_back(package_pos);

  dr->Update_Drone_To_Package_Route(drone_to_pack);
  // dr->SetRoutes(drone_to_pack, pack_to_cust);
}

void csci3081::Scheduling_System::Compute_Paths_To_Customers(
    csci3081::Drone* dr, csci3081::Package* pack, csci3081::Customer* cust,
    entity_project::IGraph* graph, int type) {
  // std::cout << "Within compute paths to customer" << std::endl;

  entity_project::SimpleUMNRoutePoint dr_pos =
      entity_project::SimpleUMNRoutePoint(
          dr->GetPosition()[0], dr->GetPosition()[1], dr->GetPosition()[2]);

  entity_project::SimpleUMNRoutePoint cust_pos =
      entity_project::SimpleUMNRoutePoint(cust->GetPosition()[0],
                                          cust->GetPosition()[1],
                                          cust->GetPosition()[2]);

  entity_project::SimpleUMNRoutePoint package_pos =
      entity_project::SimpleUMNRoutePoint(pack->GetPosition()[0],
                                          pack->GetPosition()[1],
                                          pack->GetPosition()[2]);

  csci3081::Dijkstra ds;

  // entity_project::IGraphNode* dr_node = ds.FindNodeFromEntityPosition(dr_pos,
  // graph);
  entity_project::IGraphNode* pack_node =
      ds.FindNodeFromEntityPosition(package_pos, graph);
  entity_project::IGraphNode* cust_node =
      ds.FindNodeFromEntityPosition(cust_pos, graph);

  vector<entity_project::IGraphNode*> package_to_customer_nodes;
  if (package_pos.DistanceBetween(cust_pos) >= 0.01) {
    package_to_customer_nodes = ds.ShortestPath(pack_node, cust_node, graph, type);
  }
  std::vector<std::string> current_route;
  if (package_pos.DistanceBetween(cust_pos) >= 0.01) {
    for (int i = 0; i < package_to_customer_nodes.size(); i++) {
      current_route.push_back(package_to_customer_nodes[i]->GetName());
      // std::cout << package_to_customer_nodes[i]->GetPosition().at(0) << " "
      // << package_to_customer_nodes[i]->GetPosition().at(1) << " " <<
      // package_to_customer_nodes[i]->GetPosition().at(2) << std::endl;
    }
  }
  // dr->SetCurrentRoute(current_route);
  std::vector<entity_project::SimpleUMNRoutePoint> pack_to_cust =
      ds.PathToRoute(package_to_customer_nodes);

  pack_to_cust.push_back(cust_pos);

  dr->Update_Package_To_Customer_Route(pack_to_cust);
  // std::cout << "routes created" << std::endl;
}
