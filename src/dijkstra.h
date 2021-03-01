#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <EntityProject/simple_UMN_route_manager.h>
#include <bits/stdc++.h>

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "EntityProject/graph.h"
#include <cmath>
namespace csci3081 {

struct NodeInfo {
  entity_project::IGraphNode* self;
  entity_project::IGraphNode* prev;
  float g;
  float f;
  bool isOpen, isClosed;
  NodeInfo() {}
};
/**
 * @brief A class to allow for my own priority queue type
 * which uses NodeInfo structs and compares their dist values.
 */
class NodeCompare {
 public:
  /**
   * An overloaded comparison operator to compare the dist values of two
   * NodeInfo pointers.
   * @param[in] n1 is a NodeInfo*.
   * @param[in] n2 is a NodeInfo*.
   * @return A boolean value that is true if n1 has a lower dist value than n2,
   * false otherwise.
   */
  bool operator()(const csci3081::NodeInfo* n1, const csci3081::NodeInfo* n2) {
    if (n1->f > n2->f) {
      return true;
    } else {
      return false;
    }
  }
};
typedef std::priority_queue<NodeInfo*, std::vector<NodeInfo*>, NodeCompare>
    mypq_type;

/**
 * @brief A class to compute the shortest path between two entities.
 * Utilizes Dijkstra's algorithm to compute the shortest path.
 */
class Dijkstra {
 public:
  /**
   * Default constructor for a Dijkstra object.
   * @return Reference to a Dijkstra object.
   */
  Dijkstra() {}
  /**
   * A function to compute the shortest path between two nodes, given a graph of
   * nodes.
   * @param[in] start is the starting node for the path.
   * @param[in] end is the ending node for the path.
   * @param[in] graph is a graph of nodes that is used to create the shortest
   * path.
   * @param[in] type a flag to indicate the type of A* algorithm to use.
   * @return A vector of IGraphNode* representing a shortest path between the
   * start and end nodes.
   */
  std::vector<entity_project::IGraphNode*> ShortestPath(
      entity_project::IGraphNode* start, entity_project::IGraphNode* end,
      entity_project::IGraph* graph, int type);
  /**
   * A function to translate a vector of IGraphNode* into SimpleUMNRoutePoints
   * for usage by the Drone.
   * @param[in] path A vector of IGraphNode* that represents the shortest path
   * between the start and end nodes.
   * @return A vector of SimpleUMNRoutePoints that represents the shortest path
   * between the start and end nodes.
   */
  std::vector<entity_project::SimpleUMNRoutePoint> PathToRoute(
      std::vector<entity_project::IGraphNode*> path);
  /**
   * A function that finds the closest node to a given point. Used to calculate
   * the start and end nodes.
   * @param[in] point is a SimpleUMNRoutePoint that we are trying to represent
   * with a node.
   * @param[in] graph is a graph of nodes.
   * @return An IGraphNode* that is the closest node in the graph to the point.
   */
  entity_project::IGraphNode* FindNodeFromEntityPosition(
      entity_project::SimpleUMNRoutePoint point, entity_project::IGraph* graph);

  /**
   * A helper function for Dijkstra's algorithm that produces the distance
   * between two nodes.
   * @param[in] n1 is an IGraphNode*.
   * @param[in] n2 is an IGraphNode*.
   * @return a float value that is the Euclidean distance between the two nodes.
   */
  float DistanceBetween(entity_project::IGraphNode* n1,
                        entity_project::IGraphNode* n2);

  /**
   * This function updates f(=g) of each node using dijkstra algorithm.
   * It has no heuristic and uses euclidean distance as g (graphType = 0).
   * @param[in] node is the current node.
   * @param[in] prevNode is a node that's the parent of node.
   */
  void euclidean_no_heuristic(NodeInfo* node, NodeInfo* prevNode) {
    double h = 0;
    node->g = prevNode->g + DistanceBetween(prevNode->self, node->self);
    node->f = node->g + h;
  }

  /**
   * This function updates f(=g+h) of current node using dijkstra algorithm.
   * It has no heuristic and uses euclidean distance as g (graphType = 1).
   * @param[in] node is the current node.
   * @param[in] prevNode is a node that's the parent of node.
   */
  void euclidean_neighbor_heuristic(csci3081::NodeInfo* node,
                                      csci3081::NodeInfo* prevNode) {
    double h = node->self->GetNeighbors().size();
    node->g = prevNode->g + 0.01*DistanceBetween(prevNode->self, node->self);
    node->f = node->g + h;
  }

  /**
   * This function updates f(=g+h) of current node using 
   * euclidean distance as g and h (graphType = 2).
   * @param[in] node is the current node.
   * @param[in] prevNode is a node that's the parent of node.
   * @param[in] end is a node that's the node representing the destination.
   */
  void euclidean_euclidean_heuristic(csci3081::NodeInfo* node,
                                      csci3081::NodeInfo* prevNode,
                                      entity_project::IGraphNode* end) {
    double h = DistanceBetween(node->self, end);
    node->g = prevNode->g + DistanceBetween(prevNode->self, node->self);
    node->f = node->g + h;
  }

  /**
   * This function updates f(=g+h) of current node using 
   * number of node's neighbors as heuristic (h) and the average of 
   * two nodes' number of neighbors g (graphType = 3).
   * @param[in] node is the current node.
   * @param[in] prevNode is a node that's the parent of node.
   */
  void neighbor_neighbor_heuristic(NodeInfo* node, NodeInfo* prevNode) {
    double h = node->self->GetNeighbors().size();
    node->g = prevNode->g + (prevNode->self->GetNeighbors().size()
              + node->self->GetNeighbors().size())/2.0;
    node->f = node->g + h;
  }

  /**
   * This function updates f(=g) of current node using 
   * euclidean distance as heuristic and uses the average 
   * of two nodes' number of neighbors as g (graphType = 4).
   * @param[in] node is the current node.
   * @param[in] prevNode is a node that's the parent of node.
   * @param[in] end is a node that's the node representing the destination.
   */
  void neighbor_euclidean_heuristic(csci3081::NodeInfo* node,
                                      csci3081::NodeInfo* prevNode,
                                      entity_project::IGraphNode* end) {
    double h = 0.01*DistanceBetween(node->self, end);
    node->g = prevNode->g + (prevNode->self->GetNeighbors().size()
              + node->self->GetNeighbors().size())/2.0;
    node->f = node->g + h;
  }

  /// The number of visited nodes
  int totalnodes = 0;
  int nodes_in_graph = 0;
};

}  // namespace csci3081
#endif  // DIJKSTRA_H_
