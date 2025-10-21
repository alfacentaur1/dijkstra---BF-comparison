//
// Created by filip on 20.10.2025.
//

#include "MainHelpers.h"
#include <fstream>
#include <limits>
#include <iostream>
#include <sstream>


using namespace std;

int readIntInRange(const string &prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minValue && value <= maxValue) return value;
            else {
                cerr << "Invalid input — enter a number between " << minValue << " and " << maxValue << ".\n";
                exit(1);
            }
        } else {
            cerr << "Invalid input — please enter an integer.\n";
            exit(1);
        }
    }
}

int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) return value;
        else {
            cerr << "Invalid input — please enter a positive integer.\n";
            exit(1);
        }
    }

}

Graph loadGraphFromArgs(int argc, char* argv[], int startIndex, int& outVertices) {
    if (startIndex >= argc) {
        cerr << "Error: Missing number of vertices for --manual mode.\n";
        exit(1);
    }
    // read number of vertices
    try {
        outVertices = stoi(argv[startIndex]);
    } catch (...) {
        cerr << "Error: Number of vertices must be an integer.\n";
        exit(1);
    }
    // validate number of vertices
    if (outVertices <= 0) {
        cerr << "Error: Number of vertices must be positive.\n";
        exit(1);
    }
    //now read edges based on indexing
    Graph g(outVertices);
    int edgeIndex = startIndex + 1;
    while (edgeIndex + 2 < argc) {
        string arg = argv[edgeIndex];
        if (arg.rfind("-", 0) == 0) break;

        int u, v, w;
        try {
            u = stoi(argv[edgeIndex]);
            v = stoi(argv[edgeIndex + 1]);
            w = stoi(argv[edgeIndex + 2]);
        } catch (...) {
            cerr << "Error: Edge arguments must be integers.\n";
            exit(1);
        }

        if (u < 0 || u >= outVertices || v < 0 || v >= outVertices) {
            cerr << "Error: Edge vertices must be in range 0-" << outVertices - 1 << ".\n";
            exit(1);
        }

        g.addEdge(u, v, w);
        edgeIndex += 3;
    }

    return g;
}

Graph loadGraphFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin) { cerr << "Cannot open file " << filename << endl; exit(1); }

    vector<pair<int,int>> edges;
    vector<int> weights;
    int maxVertex = -1;
    int u, v, w;

    while (fin >> u >> v >> w) {
        edges.push_back({u,v});
        weights.push_back(w);
        if (u > maxVertex) maxVertex = u;
        if (v > maxVertex) maxVertex = v;
    }

    Graph g(maxVertex + 1);
    for (size_t i = 0; i < edges.size(); ++i)
        g.addEdge(edges[i].first, edges[i].second, weights[i]);

    return g;
}

void loadGraphManual(Graph& g) {
    int numberOfEdges = readInt("Enter number of edges: ");
    cout << "Enter each edge as: u v w\n";

    for (int i = 0; i < numberOfEdges; ) {
        int u, v, w;

        cout << "Edge " << i + 1 << " - from (0-" << g.getSize()-1 << "): ";
        if (!(cin >> u)) {
            cerr << "Invalid input — please enter an integer.\n";
            exit(1);
        }

        cout << "Edge " << i + 1 << " - to (0-" << g.getSize()-1 << "): ";
        if (!(cin >> v)) {
            cerr << "Invalid input — please enter an integer.\n";
            exit(1);
        }

        if (u < 0 || u >= g.getSize() || v < 0 || v >= g.getSize()) {
            cerr << "Error: vertexes must be in range 0-" << g.getSize()-1 << ". Try again.\n";
            exit(1);
        }

        cout << "Edge " << i + 1 << " - weight: ";
        if (!(cin >> w)) {
            cerr << "Invalid input — please enter an integer.\n";
            exit(1);
        }

        g.addEdge(u, v, w);
        i++;
    }
}

int getInputVertices() {
    return readIntInRange("Enter number of vertices: ", 1, 10000);
}

void helperFunction() {
    cout << "Usage:\n"
         << "  --file <filename> --algo <dijkstra|bellman>\n"
         << "  --stdin --algo <dijkstra|bellman>\n"
         << "  --manual <num_vertices> <edges...> --algo <dijkstra|bellman>\n"
         << "  --help\n\n"
         << "Options:\n"
         << "  --file <filename>      Load graph from file (each line: u v w)\n"
         << "  --stdin                Read graph interactively from keyboard\n"
         << "  --manual <num_vertices> <edges...>\n"
         << "                        Provide graph directly via command line.\n"
         << "                        First argument is number of vertices.\n"
         << "                        Then list edges as triples: u v w\n"
         << "                        Example:\n"
         << "                          --manual 5 0 1 10 1 2 20 2 3 15 3 4 30 --algo dijkstra\n"
         << "                        Note: Make sure the graph is connected between start and end vertices.\n"
         << "  --algo <name>          Choose algorithm: dijkstra or bellman\n"
         << "  --help                 Show this help message and exit\n";
}
