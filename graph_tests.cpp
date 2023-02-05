#include "graph.h"
#include <iostream>

using std::cout, std::endl;

#define black   "\033[30m"
#define red     "\033[31m"
#define green   "\033[32m"
#define yellow  "\033[33m"
#define blue    "\033[34m"
#define magenta "\033[35m"
#define cyan    "\033[36m"
#define white   "\033[37m"
#define reset   "\033[m"

#define to_be ==
#define not_to_be !=
#define is to_be
#define is_not not_to_be

#define expect(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << "." << reset << std::endl;\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " threw an unexpected exception." << reset << std::endl;\
}

#define assert(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "failed assertion that " << #X << ". (aborting)" << reset << std::endl;\
    std::abort();\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " assertion threw an unexpected exception." << reset << std::endl;\
}

#define expect_throw(X,E) {\
  bool threw_expected_exception = false;\
  try { X; }\
  catch(const E& err) {\
    threw_expected_exception = true;\
  } catch(...) {\
    std::cout << blue << "  [help]" << reset << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << blue << #X << " threw an incorrect exception." << reset << std::endl;\
  }\
  if (!threw_expected_exception) {\
    std::cout << red <<"  [fail]" << reset << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << " to throw " << #E <<"." << reset <<std::endl;\
  }\
}

#define expect_no_throw(X) {\
  try { X; }\
  catch(...) {\
    std::cout << red << "  [fail]" << red << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << " not to throw an excpetion." << reset << std::endl;\
  }\
}


void compile_test() {
  std::cout << "begin compile_test" << std::endl;
  std::cout << "make an empty digraph" << std::endl;
  Graph G;

  std::cout << "add vertices" << std::endl;
  for (size_t n = 1; n <= 7; n++) {
      G.add_vertex(n);
  }

  std::cout << "running contains_vertex" << std::endl;
  for (size_t n = 1; n <= 7; n++) {
      G.contains_vertex(n);
  }
  G.contains_vertex(20); // does not exist

  std::cout << "add directed edges" << std::endl;
  G.add_edge(1,2,5);  // 1 ->{5} 2; (edge from 1 to 2 with weight 5)
  G.add_edge(1,3,3);
  G.add_edge(2,3,2);
  G.add_edge(2,5,3);
  G.add_edge(2,7,1);
  G.add_edge(3,4,7);
  G.add_edge(3,5,7);
  G.add_edge(4,1,2);
  G.add_edge(4,6,6);
  G.add_edge(5,4,2);
  G.add_edge(5,6,1);
  G.add_edge(7,5,1);

  std::cout << "running contains_edge" << std::endl;
  G.contains_edge(1, 2);
  G.contains_edge(1, 3);
  G.contains_edge(2, 3);
  G.contains_edge(2, 5);
  G.contains_edge(2, 7);
  G.contains_edge(3, 4);
  G.contains_edge(3, 5);
  G.contains_edge(4, 1);
  G.contains_edge(4, 6);
  G.contains_edge(5, 4);
  G.contains_edge(5, 6);
  G.contains_edge(7, 5);
  G.contains_edge(20, 1); // does not exist

  std::cout << "G has " << G.vertex_count() << " vertices" << std::endl;
  std::cout << "G has " << G.edge_count() << " edges" << std::endl;

  std::cout << "removing a vertex" << std::endl;
  G.remove_vertex(2);
  std::cout << "G has " << G.vertex_count() << " vertices" << std::endl;
  std::cout << "removing an edge" << std::endl;
  G.remove_edge(1, 2);
  std::cout << "G has " << G.edge_count() << " edges" << std::endl;

  std::cout << "running dijkstra" << std::endl;
  G.dijkstra(1); // will call distance - no need to call it here
  std::cout << "printing shortest path" << std::endl;
  G.print_shortest_path(3);

  std::cout << "end compile_test" << std::endl;
}

void graph_correctness() {
  Graph g;
  // new insert
  expect(g.add_vertex(1) to_be true);
  expect(g.vertex_count() to_be 1);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be false);
  expect(g.edge_count() to_be 0);

  // duplicate insert
  expect(g.add_vertex(1) to_be false);
  expect(g.vertex_count() to_be 1);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be false);
  expect(g.edge_count() to_be 0);

  // new insert
  expect(g.add_vertex(2) to_be true);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.edge_count() to_be 0);
  expect(g.contains_edge(1, 2) to_be false);

  // add edge between two existing vertices
  expect(g.add_edge(1, 2) to_be true);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 1);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_edge(1, 2) to_be true);
  expect(g.contains_edge(2, 1) to_be false);

  // duplicate add edge
  expect(g.add_edge(1, 2) to_be false);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 1);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_edge(1, 2) to_be true);
  expect(g.contains_edge(2, 1) to_be false);

  // remove that edge
  expect(g.remove_edge(1, 2) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 0);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);

  // attempt to remove the edge again
  expect(g.remove_edge(1, 2) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 0);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);

  // add edge between only 1 existing vertex
  expect(g.add_edge(1, 3) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_vertex(3) to_be false);
  expect(g.edge_count() to_be 0);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(1, 3) to_be false);
  expect(g.contains_edge(3, 1) to_be false);
  expect(g.contains_edge(2, 3) to_be false);
  expect(g.contains_edge(3, 2) to_be false);

  // add edge between two vertices which do not exist
  expect(g.add_edge(4, 5) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_vertex(4) to_be false);
  expect(g.contains_vertex(5) to_be false);
  expect(g.edge_count() to_be 0);
  expect(g.contains_edge(1, 1) to_be false);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(1, 4) to_be false);
  expect(g.contains_edge(1, 5) to_be false);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(2, 2) to_be false);
  expect(g.contains_edge(2, 4) to_be false);
  expect(g.contains_edge(2, 5) to_be false);
  expect(g.contains_edge(4, 1) to_be false);
  expect(g.contains_edge(4, 2) to_be false);
  expect(g.contains_edge(4, 4) to_be false);
  expect(g.contains_edge(4, 5) to_be false);
  expect(g.contains_edge(5, 1) to_be false);
  expect(g.contains_edge(5, 2) to_be false);
  expect(g.contains_edge(5, 4) to_be false);
  expect(g.contains_edge(5, 5) to_be false);

  // add a self directed edge
  expect(g.add_edge(1, 1) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 1);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(2, 2) to_be false);

  // confirm the costs of all the live edges
  expect(g.cost(1, 1) to_be 1.0);
  // expect cost of any other edge to be infinity (doesn't exist)
  expect(g.cost(1, 2) to_be INFINITY);
  expect(g.cost(2, 1) to_be INFINITY);
  expect(g.cost(2, 2) to_be INFINITY);

  // insert a new edge with specified weight
  expect(g.add_edge(2, 1, 27.0) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.edge_count() to_be 2);
  // confirm cost of live edges
  expect(g.cost(1, 1) to_be 1.0);
  expect(g.cost(2, 1) to_be 27.0);
  // expect cost of any other edge to be infinity (doesn't exist)
  expect(g.cost(1, 2) to_be INFINITY);
  expect(g.cost(2, 2) to_be INFINITY);

  // remove an edge which does not exist (because one of the vertices do not exist)
  expect(g.remove_edge(6, 1) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.contains_edge(2, 1) to_be true);
  expect(g.contains_edge(5, 1) to_be false);
  expect(g.edge_count() to_be 2);

  // remove an edge which does not exist (both vertices exist but there is no edge between them)
  expect(g.remove_edge(2, 2) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.contains_edge(2, 1) to_be true);
  expect(g.contains_edge(2, 2) to_be false);
  expect(g.edge_count() to_be 2);

  // remove an edge
  expect(g.remove_edge(2, 1) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.cost(2, 1) to_be INFINITY);
  expect(g.edge_count() to_be 1);

  // remove a vertex which does not exist
  expect(g.remove_vertex(9) to_be false);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.edge_count() to_be 1);

  // insert one more edge before removing this vertex to ensure all edges get removed when that vertex is removed
  expect(g.add_edge(1, 2) to_be true);
  expect(g.vertex_count() to_be 2);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be true);
  expect(g.contains_edge(1, 2) to_be true);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(2, 2) to_be false);
  expect(g.edge_count() to_be 2);

  // remove vertex with edges
  expect(g.remove_vertex(1) to_be true);
  expect(g.vertex_count() to_be 1);
  expect(g.contains_vertex(1) to_be false);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_edge(1, 1) to_be false);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(2, 2) to_be false);
  expect(g.edge_count() to_be 0);

  // remove vertex with no edges
  expect(g.remove_vertex(2) to_be true);
  expect(g.vertex_count() to_be 0);
  expect(g.contains_vertex(1) to_be false);
  expect(g.contains_vertex(2) to_be false);
  expect(g.contains_edge(1, 1) to_be false);
  expect(g.contains_edge(1, 2) to_be false);
  expect(g.contains_edge(2, 1) to_be false);
  expect(g.contains_edge(2, 2) to_be false);
  expect(g.edge_count() to_be 0);

}

/*
void rule_of_three () {
  std::cout << std::endl << "begin rule_of_three" << std::endl;
  // requires internal data members to be public - will not compile otherwise

  Graph g;
  g.add_vertex(1);
  g.add_vertex(2);
  g.add_vertex(3);
  g.add_vertex(4);
  g.add_vertex(5);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  g.add_edge(5, 1);
  expect(g.vertex_count() to_be 5);
  expect(g.edge_count() to_be 5);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_vertex(3) to_be true);
  expect(g.contains_vertex(4) to_be true);
  expect(g.contains_vertex(5) to_be true);
  expect(g.contains_edge(1, 2) to_be true);
  expect(g.contains_edge(2, 3) to_be true);
  expect(g.contains_edge(3, 4) to_be true);
  expect(g.contains_edge(4, 5) to_be true);
  expect(g.contains_edge(5, 1) to_be true);

  // self assign test
  g = g;
  expect(g.vertex_count() to_be 5);
  expect(g.edge_count() to_be 5);
  expect(g.contains_vertex(1) to_be true);
  expect(g.contains_vertex(2) to_be true);
  expect(g.contains_vertex(3) to_be true);
  expect(g.contains_vertex(4) to_be true);
  expect(g.contains_vertex(5) to_be true);
  expect(g.contains_edge(1, 2) to_be true);
  expect(g.contains_edge(2, 3) to_be true);
  expect(g.contains_edge(3, 4) to_be true);
  expect(g.contains_edge(4, 5) to_be true);
  expect(g.contains_edge(5, 1) to_be true);

  // display edges
  for (const auto& p : g.graph) {
    for (const auto& adj : p.second->adj_list) {
      cout << "edge from " << p.second->ID << " to " << adj.first << " with edge weight " << adj.second << std::endl;
    }
  }

  Graph G = g;
  expect(G.vertex_count() to_be 5);
  expect(G.edge_count() to_be 5);
  expect(G.contains_vertex(1) to_be true);
  expect(G.contains_vertex(2) to_be true);
  expect(G.contains_vertex(3) to_be true);
  expect(G.contains_vertex(4) to_be true);
  expect(G.contains_vertex(5) to_be true);
  expect(G.contains_edge(1, 2) to_be true);
  expect(G.contains_edge(2, 3) to_be true);
  expect(G.contains_edge(3, 4) to_be true);
  expect(G.contains_edge(4, 5) to_be true);
  expect(G.contains_edge(5, 1) to_be true);

  // display edges
  for (const auto& p : G.graph) {
    for (const auto& adj : p.second->adj_list) {
      cout << "edge from " << p.second->ID << " to " << adj.first << " with edge weight " << adj.second << std::endl;
    }
  }

  std::cout << "end rule_of_three" << std::endl;
}
*/

void dijkstra() {
  std::cout << std::endl << "begin dijkstra" << std::endl;
  Graph G;
  for (size_t n = 1; n <= 7; n++) {
        G.add_vertex(n);
    }

    G.add_edge(1,2,2);
    G.add_edge(1,4,1);
    G.add_edge(2,4,3);
    G.add_edge(2,5,10);
    G.add_edge(3,1,4);
    G.add_edge(3,6,5);
    G.add_edge(4,3,2);
    G.add_edge(4,6,8);
    G.add_edge(4,7,4);
    G.add_edge(4,5,2);
    G.add_edge(5,7,6);
    G.add_edge(7,6,1);

    G.dijkstra(2);
    /*
      expect:
      src -> dest is distance
      2 -> 1 is 9
      2 -> 2 is 0
      2 -> 3 is 5
      2 -> 4 is 3
      2 -> 5 is 5
      2 -> 6 is 8
      2 -> 7 is 7
    */
    for (size_t n = 1; n <= 7; n++) {
        std::cout << "shortest path from 2 to " << n << std::endl;
        std::cout << "  ";
        G.print_shortest_path(n);
    }
    cout << endl;


    G.dijkstra(1);
    /*
      expect:
      src -> dest is distance
      1 -> 1 is 0
      1 -> 2 is 2
      1 -> 3 is 3
      1 -> 4 is 1
      1 -> 5 is 3
      1 -> 6 is 6
      1 -> 7 is 5

    */
    for (size_t n = 1; n <= 7; n++) {
        std::cout << "shortest path from 1 to " << n << std::endl;
        std::cout << "  ";
        G.print_shortest_path(n);
    }

  std::cout << "end dijkstra" << std::endl;
}

/*
void internal_dijkstra() {
  // requires internal data members to be public - will not compile otherwise

  Graph G;

  for (size_t n = 1; n <= 7; n++) {
      G.add_vertex(n);
  }
  G.add_edge(1,2,5);
  G.add_edge(1,3,3);
  G.add_edge(2,3,2);
  G.add_edge(2,5,3);
  G.add_edge(2,7,1);
  G.add_edge(3,4,7);
  G.add_edge(3,5,7);
  G.add_edge(4,1,2);
  G.add_edge(4,6,6);
  G.add_edge(5,4,2);
  G.add_edge(5,6,1);
  G.add_edge(7,5,1);
  G.dijkstra(2);

  expect(G.graph.at(1)->predecessor->ID to_be 4);
  expect(G.graph.at(1)->distance to_be 6);
  expect(G.graph.at(2)->predecessor to_be nullptr);
  expect(G.graph.at(2)->distance to_be 0);
  expect(G.graph.at(3)->predecessor->ID to_be 2);
  expect(G.graph.at(3)->distance to_be 2);
  expect(G.graph.at(4)->predecessor->ID to_be 5);
  expect(G.graph.at(4)->distance to_be 4);
  expect(G.graph.at(5)->predecessor->ID to_be 7);
  expect(G.graph.at(5)->distance to_be 2);
  expect(G.graph.at(6)->predecessor->ID to_be 5);
  expect(G.graph.at(6)->distance to_be 3);
  expect(G.graph.at(7)->predecessor->ID to_be 2);
  expect(G.graph.at(7)->distance to_be 1);

  expect(G.distance(1) to_be 6);
  expect(G.distance(2) to_be 0);
  expect(G.distance(3) to_be 2);
  expect(G.distance(4) to_be 4);
  expect(G.distance(5) to_be 2);
  expect(G.distance(6) to_be 3);
  expect(G.distance(7) to_be 1);
}
*/


int main() {

  compile_test();
  graph_correctness();
  // rule_of_three(); // requires that internal data members are public for access - will not compile when private
  dijkstra();
  // internal_dijkstra(); // requires that internal data members are public for access - will not compile when private
    
  return 0;
}
