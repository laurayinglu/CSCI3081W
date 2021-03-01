/*! \mainpage Drone Delivery Project
 *
 * \section intro_sec Introduction of project
 * 
 * ## Timeline
 * 
 * Timeline: 10/30 Finished the prototype for drone delivery system
 * 
 * 11/22 Finished the ANVIL Simulation Specification 
 * 
 * ### Iteration One
 * During the process of Iteration one, a prototype of the simple drone delivery
 * system is developed. Bascially, simulation, visualization and ecommerce are combined 
 * together for the prototype. The test location for the initial system is at the University
 * of Minnesota. Specifically, as is mentioned in our class material, in our simulation, 
 * an available drone first goes to grab the package and then delivers to the customer.
 * 
 * ### Iteration Two
 * During the process of Iteration two, now we have different drone models to choose from.
 * For ANVIL2, we are using real roadway data from the OpenStreetMap(OSM) application
 * to build a 3D grid of publicly available routes a drone can legally traverse. We also use 
 * Dijkstra algorithm to compute the shortest path. Finally, 
 * our drones can deliver packages from any point on the map to a customer anywhere
 * on the map. Multiple drones should be delivering different sized packages at different speeds.
 * At end, the visual and other observers should be notified of package delivery events.
 * 
 * ### Iteration Three
 * In the third iteration, our team developed several variations of A* pathfinding to evaluate their 
 * efficacy and efficiency. A* is a pathfinding algorithm that improves upon Dijkstra's shortest
 *  path by including heuristics, calculations that are meant to inform the pathfinder’s next 
 * choice with a sense of how close each of their options are to the ultimate goal. The 
 * algorithms we used varied in how they calculated distance traveled “so far”, and how they
 *  estimated distance remaining. Each of these could be based on either Euclidean distances, 
 * or on the number of neighbors a node had, taken as a proxy for its “congestion.” This
 *  two-by-two variation left us with four pathfinding algorithms to compare.
 * 
 * The four variations were as follows:
 * 1. A “Euclidan-Neighbor heuristic,” which calculates the position based on Euclidean calculations and estimates the cost of the next node based on its number of neighbors.
 * 2. A “Euclidan-Euclidean heuristic,” which calculates the position based on Euclidean calculations and estimates the cost of the next node based on Euclidean distance to the end.
 * 3. A “Neighbor-Neighbor heuristic,” which calculates the position based on the number of neighbors, and estimates the cost of the next node based on its number of neighbors.
 * 4. A “Neighbor-Euclidean heuristic,” which calculates the position based on the  number of neighbors and estimates the cost of the next node based on Euclidean distance to the end.
 * 
 * The efficiency of each of these methods are measured in compute time, percentage of graph 
 * nodes visited, and total path length. Also, we kept our implementation of Dijkstra’s 
 * for comparison. So, to see if a given graph and heuristic created a shortest path,
 *  compare to the path length of graph 0. 
 * 
 * ## Key classes/design patterns:
 * 
 * The documentation gives an overview of the project.
 * The facade is the Drone_Simulation class. The subsystems used by the
 * Drone_Simulation class are: The Entity_Factory class which creates entities
 * via a factory pattern, The Scheduling_System class which schedules deliveries
 * by giving the Customer and Package information to the Drone, and the
 * Movement_System class which moves the Drone and Package according to the
 * route made by the Scheduling System.
 * 
 * ### UML diagram
 * \image html img/UML.png
 * 
 * ## How to compile and run
 * 
 * 1. git clone the whole project folder to your local laptop
 * 
 * 2. set the docker and start the docker container into 
 * lab environment: ./bin/run-env.sh
 * 
 * 3. navigate to the project dir: cd project
 * 
 * 4. build the simulation and the visualization: make
 * 
 * 5. Run the visualization: ./bin/run.sh
 * 
 * 6. navigate to the following link on your browser (eg. Firefox): http://127.0.0.1:8081
 * 
 * 7. Graph performance statistics will be logged to stdout. 
 * 
 */
