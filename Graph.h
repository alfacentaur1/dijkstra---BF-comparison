//
// Created by filip on 19.10.2025.
//

#ifndef COURSEWORK_GRAPH_H
#define COURSEWORK_GRAPH_H

#include <vector>
using namespace std;

struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    vector<vector<Edge>> adjList; // adjacency list representation
    int n; // number of vertices
public:
    // init adjlist to n - else segfault
    Graph(const int& n);

    // method for getting neighbors of a vertex
    // returns vector of pairs (neighbor vertex, weight)
    // if vertex is invalid, returns vector with one pair (-1,-1)
    // else push values to returnValues and return it
    vector<pair<int,int>> getNeighbors(int vertex) const;

    // getters
    int getSize() const;
    const vector<vector<Edge>>& getAdjList() const;

    // method for adding edges to the graph
    // from - starting vertex
    // to - ending vertex
    // weight - weight of the edge
    void addEdge(int from, int to, int weight);
};

#endif //COURSEWORK_GRAPH_H
