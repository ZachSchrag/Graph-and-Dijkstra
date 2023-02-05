/*
*   Directed graph implemented using adjacency lists with an implementation of Dijkstra's algorithm 
*   Written by Zach Schrag
*/

#pragma once
#include <unordered_map>
#include <cmath> // INFINITY
#include <queue> // dijkstra
#include <stack> // print_shortest_path
#include <iostream> // print_shortest_path

class Graph {
    struct Vertex {
        size_t ID;
        std::unordered_map<size_t, double> adj_list;
        bool visited;
        double distance;
        Vertex* predecessor;

        Vertex() : ID{}, adj_list{}, visited{}, distance{}, predecessor{} {}
        explicit Vertex(size_t ID) : ID{ID}, adj_list{}, visited{false}, distance{0}, predecessor{nullptr} {}
        bool operator<(const Vertex& other) { return this->distance < other.distance; }

        void copy(const Vertex& other) {
            ID = other.ID;
            visited = other.visited;
            adj_list = other.adj_list;
            distance = other.distance;
            other.predecessor ? predecessor = new Vertex(other.predecessor->ID) : predecessor = nullptr;
        }

        Vertex(const Vertex& other) : Vertex() { copy(other); }
        Vertex& operator=(const Vertex& rhs) { *this = Vertex(rhs); return *this; }
    };

    std::unordered_map<size_t, Vertex*> graph;
    size_t edges; // number of edges counter - want to return edge_count in constant time
    
    public:
    // constructor
    Graph() : graph{}, edges{0} {}

    // rule of three
    void clear() {
        for (const std::pair<size_t, Vertex*>& pair : graph) {
            delete pair.second;
        }
        graph.clear();
        edges = 0;
    }

    void copy(const Graph& source) {
        edges = source.edges;
        for (const std::pair<size_t, Vertex*>& pair : source.graph) {
            Vertex* vertex = new Vertex(*pair.second);
            graph.insert(std::pair<size_t, Vertex*>(pair.first, vertex));
        }
    }

    Graph(const Graph& source) : Graph() { copy(source); }
    ~Graph() { clear(); }
    Graph& operator=(const Graph& rhs) {
        if (this != &rhs) {
            clear();
            copy(rhs);
        }
        return *this;
    }
    
    // capacity
    size_t vertex_count() const { return graph.size(); }
    size_t edge_count() const { return edges; }

    // element access
    bool contains_vertex(size_t id) const {
        return graph.find(id) != graph.end();
    }

    bool contains_edge(size_t src, size_t dest) const {
        // confirm the two vertices exist
        if (!contains_vertex(src) || !contains_vertex(dest)) return false;

        // look for edge
        Vertex* source = graph.at(src);
        return source->adj_list.find(dest) != source->adj_list.end();
    }

    double cost(size_t src, size_t dest) const {
        if (!contains_edge(src, dest)) return INFINITY; // cost to vertex which is not connected is represented as INFINITY
        // edge exist, get cost
        Vertex* source = graph.at(src);
        return source->adj_list.at(dest);
    }

    bool add_vertex(size_t id) {
        if (contains_vertex(id)) return false;
        return graph.insert(std::pair<size_t, Vertex*>{id, new Vertex(id)}).second;
    }

    bool add_edge(size_t src, size_t dest, double weight = 1.0) {
        // confirm vertices exist
        if (!contains_vertex(src) || !contains_vertex(dest)) return false;

        // confirm edge exist
        if (contains_edge(src, dest)) return false;

        // add the edge
        Vertex* source = graph.at(src);
        source->adj_list.insert(std::pair<size_t, double>{dest, weight});
        edges++;

        return true;
    }

    bool remove_edge(size_t src, size_t dest) {
        // confirm edge exists
        if (!contains_edge(src, dest)) return false;

        // remove the edge
        Vertex* source = graph.at(src);
        source->adj_list.erase(dest);
        edges--;

        return true;
    }

    bool remove_vertex(size_t id) {
        // confirm vertex exist
        if (!contains_vertex(id)) return false;

        // remove all incoming edges
        for (const std::pair<size_t, Vertex*>& pair : graph) {
            if (contains_edge(pair.first, id)) remove_edge(pair.first, id);
        }

        // remove all outgoing edges
        Vertex* source = graph.at(id);
        edges -= source->adj_list.size(); // update edges counter
        source->adj_list.clear();

        // remove requested vertex
        delete graph.at(id);
        graph.erase(id);

        return true;
    }

    // dijkstra methods
    void dijkstra(size_t src) {
        // confirm the pre condition (distance is infinity for all and no predecessor)
        for (const std::pair<size_t, Vertex*>& pair : graph) {
            pair.second->distance = INFINITY;
            pair.second->predecessor = nullptr;
            pair.second->visited = false;
        }

        if (!contains_vertex(src)) return; // confirm source vertex exists

        // finish initialization
        graph.at(src)->distance = 0;
        std::priority_queue<std::pair<double, Vertex*>, std::vector<std::pair<double, Vertex*>>, std::greater<std::pair<double, Vertex*>>> q;
        q.push(std::pair<double, Vertex*>(0, graph.at(src)));

        while (!q.empty()) {
            // grab an unknown vertex that has minimum distance over all unknown vertices
            std::pair<double, Vertex*> current = q.top();
            q.pop();

            if (current.second->visited) continue; // shortest path is known
            current.second->visited = true;

            double currDistance;
            // update the distance for all vertices adjacent to current
            for (const std::pair<size_t, double>& adj_vertex : current.second->adj_list) {
                currDistance = graph.at(adj_vertex.first)->distance;
                if (currDistance > current.second->distance + cost(current.second->ID, adj_vertex.first)) {
                    currDistance = current.second->distance + cost(current.second->ID, adj_vertex.first); // update current distance
                    graph.at(adj_vertex.first)->distance = currDistance; // update vertex distance
                    q.push(std::pair<double, Vertex*>(cost(current.second->ID, adj_vertex.first), graph.at(adj_vertex.first))); // enqueue since distance changed
                    graph.at(adj_vertex.first)->predecessor = current.second; // update predecessor
                }
            }
        }
    }

    // helper for dijkstra
    double distance(size_t id) const { 
        if (!contains_vertex(id)) return INFINITY;
        else return graph.at(id)->distance;
    }

    // visual representation
    void print_shortest_path(size_t id, std::ostream& os = std::cout) const {
        /*
         *  to be ran AFTER dijkstra has been called on any vertex
         *  prints the shortest path from the vertex passed to dijkstra to id
         *  e.g., djikstra(1) followed by print_shortest_path(3) prints shortest path from 1 -> 3 displaying entire path
        */

        // no path exists to a vertex which does not exist or one who has distance infinity after dijkstra has been ran
        if (!contains_vertex(id) || graph.at(id)->distance == INFINITY) {
            os << "<no path>" << std::endl;
            return;
        }

        Vertex* tmpPred = graph.at(id); // starting vertex
        std::stack<size_t> s; // to store path

        while(tmpPred != nullptr) {
            s.push(tmpPred->ID);
            tmpPred = tmpPred->predecessor;
        }
        
        size_t currID;
        while (!s.empty()) {
            currID = s.top();
            currID == id ? os << currID : os << currID << " --> ";
            s.pop();
        }

        os << " distance: " << distance(id) << std::endl;
    }
};
