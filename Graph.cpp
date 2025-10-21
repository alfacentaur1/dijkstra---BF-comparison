//
// Created by filip on 19.10.2025.
//

#include "Graph.h"

// constructor
Graph::Graph(const int& n) : n(n), adjList(n) {}

// method for adding edges to the graph
// from - starting vertex
// to - ending vertex
// weight - weight of the edge
void Graph::addEdge(int from, int to, int weight) {
    if (from >= 0 && from < n && to >= 0 && to < n) {
        adjList[from].push_back({to, weight});
    }
}

// method for getting neighbors of a vertex
// returns vector of pairs (neighbor vertex, weight)
// if vertex is invalid, returns vector with one pair (-1,-1)
vector<pair<int,int>> Graph::getNeighbors(int vertex) const {
    vector<pair<int,int>> returnValues = {};
    if (vertex < 0 || vertex >= n) {
        return {{-1,-1}}; // invalid vertex
    }
    for (auto edge : adjList[vertex]) {
        returnValues.push_back({edge.to, edge.weight});
    }
    return returnValues;
}

// getter for number of vertices
int Graph::getSize() const {
    return n;
}

// getter for adjacency list
const vector<vector<Edge>>& Graph::getAdjList() const {
    return adjList;
}
