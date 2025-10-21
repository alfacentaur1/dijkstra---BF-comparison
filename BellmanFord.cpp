//
// Created by filip on 19.10.2025.
//

#include "BellmanFord.h"
#include <vector>
#include <climits>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "Graph.h"
#include <iostream>
using namespace std;



//we will relax all edges V-1 times
//then we will check for negative weight cycles
pair<string,int> BellmanFord::shortestPath(const Graph& graph, int start, int end) {
    auto startTime = std::chrono::high_resolution_clock::now(); // start timing

    vector<int> distances(graph.getSize(), INT_MAX);
    distances[start] = 0;
    vector<int> parent(graph.getSize(), -1);

    //relaxation of all edges V-1 times, where V is number of vertices
    //for each vertex we will check all its neighbors and relax the edges
    //relaxation - if distance to neighbor is greater than distance to current vertex + weight of edge update distance to neighbor
    //else do nothing
    for (int i = 1; i < graph.getSize(); i++) {
        for (int u = 0; u < graph.getSize(); u++) {
            for (auto edge : graph.getNeighbors(u)) {
                if (edge.first == -1) continue;
                if (distances[u] != INT_MAX && distances[edge.first] > distances[u] + edge.second) {
                    distances[edge.first] = distances[u] + edge.second;
                    parent[edge.first] = u;
                }
            }
        }
    }
    //check for negative weight cycles
    for (int u = 0; u < graph.getSize(); u++) {
        for (auto edge : graph.getNeighbors(u)) {
            if (edge.first == -1) continue;
            if (distances[u] != INT_MAX && distances[edge.first] > distances[u] + edge.second) {
                return {"Negative weight cycle detected", -1};
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now(); // end timing
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    // Print path
    if (distances[end] == INT_MAX) return {"Unreachable", -1};
    vector<int> path;
    int current = end;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (auto v : path) {
        cout << v << " ";
    }
    cout << endl;
    cout << "Shortest distance from " << start << " to " << end << " is: " << distances[end] << endl;
    cout << "Bellman-Ford execution time: " << duration << " microseconds" << endl;
    return {"OK", distances[end]};
}
