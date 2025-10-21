//
// Created by filip on 19.10.2025.
//

#ifndef COURSEWORK_BELLMANFORD_H
#define COURSEWORK_BELLMANFORD_H
#pragma once
#include "Graph.h"
#include <string>
using namespace std;

class BellmanFord {
public:
    static pair<string,int> shortestPath(const Graph& graph, int start, int end);
};



#endif //COURSEWORK_BELLMANFORD_H