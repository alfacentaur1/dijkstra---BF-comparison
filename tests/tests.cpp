//
// Created by filip on 20.10.2025.
//

#include "tests.h"
#define CATCH_CONFIG_MAIN
#include "../Graph.h"
#include "../Dijkstra.h"
#include "../BellmanFord.h"
#include "catch.h"
#include "MainHelpers.h"
#include <climits>
# include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

//verify that the path is valid and has the expected distance
bool verifyPath(const Graph& g, const vector<int>& path, int expectedDistance) {
    if (path.empty()) return false;
    int total = 0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        bool found = false;
        for (auto e : g.getNeighbors(path[i])) {
            if (e.first == path[i + 1]) {
                total += e.second;
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return total == expectedDistance;
}

// ----------------------- DIJKSTRA TESTS ------------------------------

TEST_CASE("Dijkstra - simple graph with positive edges", "[dijkstra-basic]") {
    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 3);
    g.addEdge(2, 1, 1);
    g.addEdge(2, 3, 2);
    g.addEdge(1, 4, 6);
    g.addEdge(3, 4, 7);

    Dijkstra d;
    int result = d.shortestPath(g, 0, 4);
    REQUIRE(result == 10); // shortest path 0 -> 2 -> 3 -> 4 = 3 + 2 + 7 = 12

    vector<int> path = {0, 2, 3, 4};
    REQUIRE(verifyPath(g, path, 12));
}

TEST_CASE("Dijkstra - unreachable vertex", "[dijkstra-unreachable]") {
    Graph g(4);
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 7);
    // vertex 3 is isolated

    Dijkstra d;
    int result = d.shortestPath(g, 0, 3);
    REQUIRE(result == -1); // unreachable
}

TEST_CASE("Dijkstra - graph with cycle", "[dijkstra-cycle]") {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 0, 3); // cycle 0->1->2->0
    g.addEdge(1, 3, 10);

    Dijkstra d;
    int result = d.shortestPath(g, 0, 3);
    REQUIRE(result == 11); // 0 -> 1 -> 3
}

TEST_CASE("Dijkstra - negative edge should fail", "[dijkstra-negative]") {
    Graph g(3);
    g.addEdge(0, 1, 4);
    g.addEdge(1, 2, -5); // invalid for Dijkstra

    Dijkstra d;
    int result = d.shortestPath(g, 0, 2);
    REQUIRE(result == -1);
}

TEST_CASE("Dijkstra - large sparse graph performance", "[dijkstra-large]") {
    const int N = 200;
    Graph g(N);
    for (int i = 0; i < N - 1; i++) {
        g.addEdge(i, i + 1, 1);
    }

    Dijkstra d;
    int result = d.shortestPath(g, 0, N - 1);
    REQUIRE(result == N - 1); // linear chain
}

// --------------------- BELLMAN-FORD TESTS ----------------------------

TEST_CASE("Bellman-Ford - basic positive edges", "[bf-basic]") {
    Graph g(5);
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 3);
    g.addEdge(2, 3, 1);
    g.addEdge(3, 4, 2);

    BellmanFord bf;
    auto result = bf.shortestPath(g, 0, 4);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second == 11); // 5 + 3 + 1 + 2 = 11
}

TEST_CASE("Bellman-Ford - negative edges without cycle", "[bf-negative]") {
    Graph g(5);
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, 5);
    g.addEdge(1, 4, -4);
    g.addEdge(2, 3, -3);
    g.addEdge(3, 1, -2);
    g.addEdge(4, 0, 2);

    BellmanFord bf;
    auto result = bf.shortestPath(g, 0, 3);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second == 4); // shortest path 0→2→3 = 7 + (-3) = 4
}

TEST_CASE("Bellman-Ford - detects negative weight cycle", "[bf-cycle]") {
    Graph g(3);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, -2);
    g.addEdge(2, 0, -2); // negative cycle

    BellmanFord bf;
    auto result = bf.shortestPath(g, 0, 2);
    REQUIRE(result.first == "Negative weight cycle detected");
    REQUIRE(result.second == -1);
}

TEST_CASE("Bellman-Ford - unreachable vertex", "[bf-unreachable]") {
    Graph g(4);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    // vertex 3 is isolated

    BellmanFord bf;
    auto result = bf.shortestPath(g, 0, 3);
    REQUIRE(result.first == "Unreachable");
    REQUIRE(result.second == -1);
}

TEST_CASE("Bellman-Ford - large dense graph performance", "[bf-large]") {
    const int N = 50;
    Graph g(N);

    // fully connected graph with positive weights
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) g.addEdge(i, j, (i + j) % 10 + 1);
        }
    }

    BellmanFord bf;
    auto result = bf.shortestPath(g, 0, N - 1);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second >= 0);
}


// --------------------- File input ---------------------
TEST_CASE("Main - file input with Dijkstra", "[main-file-dijkstra]") {
    ofstream fout("test_file.txt");
    fout << "0 1 5\n0 2 2\n1 2 1\n1 3 3\n2 3 7\n";
    fout.close();

    Graph g = loadGraphFromFile("test_file.txt");
    int dist = Dijkstra::shortestPath(g, 0, 3);
    REQUIRE(dist == 8); // shortest path: 0->1->3 or 0->2->3
}

TEST_CASE("Main - file input with Bellman-Ford", "[main-file-bf]") {
    ofstream fout("test_file2.txt");
    fout << "0 1 4\n0 2 2\n1 2 3\n1 3 2\n2 3 1\n";
    fout.close();

    Graph g = loadGraphFromFile("test_file2.txt");
    auto result = BellmanFord::shortestPath(g, 0, 3);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second == 3); // shortest path: 0->2->3
}

// --------------------- Manual input ---------------------
TEST_CASE("Main - manual input Dijkstra", "[main-manual-dijkstra]") {
    Graph g(4);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 1);

    int dist = Dijkstra::shortestPath(g, 0, 3);
    REQUIRE(dist == 3); // shortest path: 0->2->3
}

TEST_CASE("Main - manual input Bellman-Ford", "[main-manual-bf]") {
    Graph g(5);
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, 5);
    g.addEdge(1, 4, -4);
    g.addEdge(2, 3, -3);
    g.addEdge(3, 1, -2);
    g.addEdge(4, 0, 2);

    auto result = BellmanFord::shortestPath(g, 0, 3);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second == 4); // shortest path: 0->2->3
}

// --------------------- Standard input ---------------------
TEST_CASE("Main - stdin simulation Dijkstra", "[main-stdin-dijkstra]") {
    istringstream input("3\n0 1 10\n1 2 5\n2 3 2\n");
    streambuf* origCin = cin.rdbuf(input.rdbuf());
    int maxVertex = 3;
    Graph g(maxVertex + 1);
    loadGraphManual(g);

    int dist = Dijkstra::shortestPath(g, 0, 3);
    REQUIRE(dist == 17);

    cin.rdbuf(origCin); // restore original stdin
}


TEST_CASE("Main - stdin simulation Bellman-Ford", "[main-stdin-bf]") {
    istringstream input("3\n0 1 1\n1 2 -1\n2 3 2\n");
    streambuf* origCin = cin.rdbuf(input.rdbuf());

    int maxVertex = 3;
    Graph g(maxVertex + 1);
    loadGraphManual(g);

    auto result = BellmanFord::shortestPath(g, 0, 3);
    REQUIRE(result.first == "OK");
    REQUIRE(result.second == 2);

    cin.rdbuf(origCin); // restore original stdin
}

// --------------------- Edge cases ---------------------
TEST_CASE("Main - unreachable vertex", "[main-unreachable]") {
    Graph g(3);
    g.addEdge(0, 1, 5);
    // vertex 2 isolated

    int dist = Dijkstra::shortestPath(g, 0, 2);
    REQUIRE(dist == -1);

    auto result = BellmanFord::shortestPath(g, 0, 2);
    REQUIRE(result.first == "Unreachable");
    REQUIRE(result.second == -1);
}

TEST_CASE("Main - negative edge Dijkstra should fail", "[main-dijkstra-negative]") {
    Graph g(2);
    g.addEdge(0, 1, -5);
    int dist = Dijkstra::shortestPath(g, 0, 1);
    REQUIRE(dist == -1);
}

// --------------------- Help function ---------------------
TEST_CASE("Main - helper function output", "[main-help]") {
    ostringstream oss;
    streambuf* origCout = cout.rdbuf(oss.rdbuf());

    helperFunction();
    string output = oss.str();
    REQUIRE(output.find("--file") != string::npos);
    REQUIRE(output.find("--algo") != string::npos);

    cout.rdbuf(origCout);
}

