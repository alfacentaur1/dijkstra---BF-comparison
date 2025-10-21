//
// Created by filip on 19.10.2025.
//
#include "MainHelpers.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "BellmanFord.h"
#include <iostream>
using namespace std;

// ---------- Main function ----------
int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Error: Missing arguments. Use --help for usage.\n";
        return 1;
    }

    string mode, algo, filename;

    int manualArgsIndex = -1; // pro loadGraphFromArgs
    // --- Parse command line arguments ---
    for (int i = 1; i < argc; ++i) {
        string argument = argv[i];
        if (argument == "--help") {
            helperFunction();
            return 0;
        }
        else if (argument == "--file" && i + 1 < argc) {
            mode = "file";
            filename = argv[++i];
        }
        else if (argument == "--stdin") {
            mode = "stdin";
        }
        else if (argument == "--manual") {
            mode = "manual";
            manualArgsIndex = i + 1;
        }
        else if (argument == "--algo" && i + 1 < argc) {
            algo = argv[++i];
        }
        else {
            // ignorujeme argumenty pro manual, jdou do loadGraphFromArgs
            if (mode != "manual") {
                cerr << "Error: Unknown or incomplete argument '" << argument << "'.\n";
                return 1;
            }
        }
    }

    if (algo.empty()) {
        cerr << "Error: Missing required --algo argument.\n";
        return 1;
    }
    if (mode.empty()) {
        cerr << "Error: Must specify one of --file, --stdin, or --manual.\n";
        return 1;
    }

    Graph graph(0); // placeholder
    int vertices = 0;

    // --- Load graph based on selected mode ---
    if (mode == "file") {
        graph = loadGraphFromFile(filename);
        vertices = graph.getSize();
    }
    else if (mode == "stdin") {
        vertices = getInputVertices();
        graph = Graph(vertices);
        loadGraphManual(graph);
    }
    else if (mode == "manual") {
        if (manualArgsIndex == -1 || manualArgsIndex >= argc) {
            cerr << "Error: No edge data provided for --manual mode.\n";
            return 1;
        }
        graph = loadGraphFromArgs(argc, argv, manualArgsIndex, vertices);
    }

    // --- Read start and end vertices safely ---
    int start = readIntInRange("Enter start vertex: ", 0, vertices-1);
    int end   = readIntInRange("Enter end vertex: ", 0, vertices-1);

    // --- Run the selected algorithm ---
    if (algo == "dijkstra") {
        int dist = Dijkstra::shortestPath(graph, start, end);
        if (dist == -1) {
            cerr << "Dijkstra: Unreachable\n";
            return 1;
        } else {
            cout << "Shortest path (" << start << " -> " << end << ") = " << dist << " [Dijkstra]\n";
        }
    }
    else if (algo == "bellman") {
        auto result = BellmanFord::shortestPath(graph, start, end);
        if (result.first == "OK")
            cout << "Shortest path (" << start << " -> " << end << ") = " << result.second << " [Bellman-Ford]\n";
        else
            cout << "Bellman-Ford: " << result.first << endl;
    }
    else {
        cerr << "Error: Unknown algorithm '" << algo << "'. Use 'dijkstra' or 'bellman'.\n";
        return 1;
    }

    return 0;
}
