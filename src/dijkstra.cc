#include "src/dijkstra.h"
#include <map>

std::vector<entity_project::IGraphNode*> csci3081::Dijkstra::ShortestPath(
    entity_project::IGraphNode* start, entity_project::IGraphNode* end,
    entity_project::IGraph* graph, int type) {
  /*std::cout << "START AND END" << std::endl;
  std::cout << start->GetName() << std::endl;
  std::cout << end->GetName() << std::endl << std::endl;*/

  // Initialize our graph
  int start_idx = -1;
  int end_idx = -1;

  std::vector<entity_project::IGraphNode*> node_queue = graph->GetNodes();
  std::vector<csci3081::NodeInfo*> nodes;
  std::map<std::string, int> node_lookup;

  for (int i = 0; i < node_queue.size(); i++) {
    csci3081::NodeInfo* ni = new csci3081::NodeInfo();
    if (node_queue.at(i)->GetName() == start->GetName()) {
      ni->self = node_queue.at(i);
      ni->f = 0.0;
      ni->g = 0.0;
      ni->prev = NULL;
      ni->isOpen = true;
      ni->isClosed = false;
      start_idx = i;
      node_lookup[node_queue.at(i)->GetName()] = i;
    } else if (node_queue.at(i)->GetName() == end->GetName()) {
      ni->self = node_queue.at(i);
      ni->f = 100000;
      ni->g = 100000;
      ni->prev = NULL;
      ni->isOpen = false;
      ni->isClosed = false;
      end_idx = i;
      node_lookup[node_queue.at(i)->GetName()] = i;
    } else {
      ni->self = node_queue.at(i);
      ni->f = 100000;
      ni->g = 100000;
      ni->prev = NULL;
      ni->isOpen = false;
      ni->isClosed = false;
      node_lookup[node_queue.at(i)->GetName()] = i;
    }
    nodes.push_back(ni);
  }

  nodes_in_graph = nodes.size();

  // initialize variables for Dijkstra's loop
  mypq_type open;
  mypq_type closed;
  open.push(nodes.at(start_idx));
  csci3081::NodeInfo* curr_node = new csci3081::NodeInfo();
  std::vector<entity_project::IGraphNode*> V_neighbors;
  entity_project::IGraphNode* curr_neighbor;
  csci3081::NodeInfo* curr_neighbor_ni;
  float temp = -100;
  int nodes_visited = 0;
  while (!open.empty()) {
    // pop the min value node off the queue and store it in curr_node
    curr_node = open.top();
    open.pop();
    nodes_visited++;

    curr_node->isClosed = true;
    curr_node->isOpen = false;
    if (curr_node->self->GetName() == end->GetName()) {
      break;
    }
    V_neighbors = curr_node->self->GetNeighbors();
    for (int i = 0; i < V_neighbors.size(); i++) {
      curr_neighbor = V_neighbors.at(i);
      curr_neighbor_ni = nodes.at(node_lookup[curr_neighbor->GetName()]);
      if (curr_neighbor_ni->isClosed) {
        continue;
      }
      if (!curr_neighbor_ni->isOpen) {
        curr_neighbor_ni->isOpen = true;
        curr_neighbor_ni->prev = curr_node->self;

        if (type == 0) {
          euclidean_no_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 1) {
          euclidean_neighbor_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 2) {
          euclidean_euclidean_heuristic(curr_neighbor_ni, curr_node, end);
        } else if (type == 3) {
          neighbor_neighbor_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 4) {
          neighbor_euclidean_heuristic(curr_neighbor_ni, curr_node, end);
        } else {
          std::cout << "graph type is not defined" << std::endl;
        }

        open.push(curr_neighbor_ni);
      }
      if (curr_neighbor_ni->isOpen && curr_node->g +
            DistanceBetween(curr_node->self, curr_neighbor_ni->self) < curr_neighbor_ni->g) {
        curr_neighbor_ni->prev = curr_node->self;

        if (type == 0) {
          euclidean_no_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 1) {
          euclidean_neighbor_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 2) {
          euclidean_euclidean_heuristic(curr_neighbor_ni, curr_node, end);
        } else if (type == 3) {
          neighbor_neighbor_heuristic(curr_neighbor_ni, curr_node);
        } else if (type == 4) {
          neighbor_euclidean_heuristic(curr_neighbor_ni, curr_node, end);
        } else {
          std::cout << "graph type is not defined" << std::endl;
        }
      }
    }
  }
  // std::cout << "Graph " << type << ": Nodes visited: " << nodes_visited << std::endl;
  totalnodes += nodes_visited;

  // reconstruct the shortest path using the prev values
  // std::cout << "Going into path reconstruction" << std::endl;
  std::vector<entity_project::IGraphNode*> shortest_path;
  if (curr_node->prev || (curr_node->self->GetName() == start->GetName())) {
    // shortest_path.push_back(end);
    while (curr_node->prev) {
      shortest_path.insert(shortest_path.begin(), curr_node->self);
      curr_node = nodes.at(node_lookup[curr_node->prev->GetName()]);
    }
    shortest_path.insert(shortest_path.begin(), start);
  }
  for (int i = 0; i < shortest_path.size(); i++) {
    // std::cout << shortest_path.at(i)->GetName() << std::endl;
  }
  return shortest_path;
}

std::vector<entity_project::SimpleUMNRoutePoint>
csci3081::Dijkstra::PathToRoute(std::vector<entity_project::IGraphNode*> path) {
  std::vector<entity_project::SimpleUMNRoutePoint> route;
  entity_project::IGraphNode* node;
  for (int i = 0; i < path.size(); i++) {
    node = path.at(i);
    std::vector<float> pos = node->GetPosition();
    route.push_back(
        entity_project::SimpleUMNRoutePoint(pos.at(0), pos.at(1), pos.at(2)));
  }
  return route;
}

float csci3081::Dijkstra::DistanceBetween(entity_project::IGraphNode* n1,
                                          entity_project::IGraphNode* n2) {
  return sqrt(pow(n1->GetPosition().at(0) - n2->GetPosition().at(0), 2.0) +
              pow(n1->GetPosition().at(1) - n2->GetPosition().at(1), 2.0) +
              pow(n1->GetPosition().at(2) - n2->GetPosition().at(2), 2.0));
}

entity_project::IGraphNode* csci3081::Dijkstra::FindNodeFromEntityPosition(
    entity_project::SimpleUMNRoutePoint point, entity_project::IGraph* graph) {
  // std::cout << "Within FindNodeFromEntityPosition" << std::endl;
  std::vector<entity_project::IGraphNode*> nodes = graph->GetNodes();
  entity_project::SimpleUMNRoutePoint node_pos(0, 0, 0);
  float min = 100000;
  entity_project::IGraphNode* min_node;

  // std::cout << "Desired...   x: " << point.x << " " << "y: " << point.y << "
  // " << "z: " << point.z << std::endl;
  for (int i = 0; i < nodes.size(); i++) {
    node_pos.x = nodes[i]->GetPosition().at(0);
    node_pos.y = nodes[i]->GetPosition().at(1);
    node_pos.z = nodes[i]->GetPosition().at(2);
    if (point.DistanceBetween(node_pos) < min) {
      min = point.DistanceBetween(node_pos);
      min_node = nodes.at(i);
    }
  }

  // std::cout << "Actual...   x: " << min_node->GetPosition().at(0) << " " <<
  // "y: " << min_node->GetPosition().at(1) << " " << "z: " <<
  // min_node->GetPosition().at(2) << std::endl;
  return min_node;
}
