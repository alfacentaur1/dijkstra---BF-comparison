//
// Created by filip on 19.10.2025.
//
#include "Dijkstra.h"
#include "Graph.h"
#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <set>
#include <algorithm>
#include <chrono>
using namespace std;


// -------------------------------------------------------------------
// Helper function: export the final graph to a DOT file
// Highlights the shortest path from 'start' to 'end' in red
// -------------------------------------------------------------------
void exportFinalGraph(const Graph& graph, int start, int end, const vector<int>& parent) {
    // Create filename with start and end to avoid collisions
    string filename = "final_graph_dijkstra" + to_string(start) + "_" + to_string(end) + ".dot";
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: cannot create DOT file " << filename << endl;
        return;
    }

    file << "digraph G {\n";

    // Collect edges in the shortest path
    set<pair<int,int>> pathEdges;
    int current = end;
    while (current != -1 && parent[current] != -1) { // include first edge
        pathEdges.insert({parent[current], current});
        current = parent[current];
    }

    // Export all edges, color shortest path edges red
    for (int u = 0; u < graph.getSize(); u++) {
        for (auto edge : graph.getNeighbors(u)) {
            int v = edge.first;
            if (v == -1) continue;
            string color = pathEdges.count({u,v}) ? "red" : "black";
            file << "    " << u << " -> " << v
                 << " [label=\"" << edge.second << "\", color=" << color << "];\n";
        }
    }

    file << "}\n";
    file.close();

    cout << "DOT graph exported to: " << filename << endl;
}

//representing gragh as adjacent list
//{} means no edge, but we can also use INF to represent no edge
int Dijkstra::shortestPath(const Graph& graph, int start, int end) {
    auto startTime = std::chrono::high_resolution_clock::now();
    vector<int> distances(graph.getSize(), INT_MAX);
    distances[start] = 0;

    //keep track of the path
    vector<int> parent(graph.getSize(), -1);

    //simulation of prio queue - track of visited vertices
    vector<bool> visited(graph.getSize(), false);
    int currentVertex = start;

    //flag for while loop - while not all vertices are visited
    int visitedCount = 0;
    int routeDistance = 0;

    //initialize distances from start vertex
    for (auto edge : graph.getNeighbors(currentVertex)) {
        if (edge.first == -1 || edge.second < 0) continue;
        distances[edge.first] = edge.second;
        parent[edge.first] = currentVertex; // set parent for first edges
    }

    //while not all vertices are visited and not all distances are evaluated - run this loop
    while (visitedCount < graph.getSize()) {
        //simulation of prio queue
        int minDistance = INT_MAX;

        //lets find the vertex with the smallest distance
        for (int i = 0; i < distances.size(); i++) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                currentVertex = i;
            }
        }

        //we are done - no more reachable vertices
        if (minDistance == INT_MAX) break;
        if (currentVertex == -1) break;

        visited[currentVertex] = true;
        visitedCount++;
        //relaxation of edges
        for (auto edge : graph.getNeighbors(currentVertex)) {
            if (edge.first == -1) continue;
            if(edge.second < 0) {
                cerr << "Error: Dijkstra cannot handle negative edge weights!" << endl;
                return -1;
            }
            if (distances[edge.first] > distances[currentVertex] + edge.second) {
                distances[edge.first] = distances[currentVertex] + edge.second;
                parent[edge.first] = currentVertex;
            }
        }

    }

    auto endTime = std::chrono::high_resolution_clock::now(); // end timing
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    // export final graph with shortest path highlighted
    exportFinalGraph(graph, start, end, parent);
    if (distances[end] == INT_MAX) {//end vertex is unreachable
        return -1;
    }

    // Print path
    vector<int> path;
    int currentVector = end;
    while (currentVector != -1) {
        path.push_back(currentVector);
        currentVector = parent[currentVector];
    }
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (auto v : path) {
        cout << v << " ";
    }
    cout << endl;

    cout << "Shortest distance from " << start << " to " << end << " is: " << distances[end] << endl;
    cout << "Dijkstra execution time: " << duration << " microseconds" << endl;

    return distances[end];
}
