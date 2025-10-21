//
// Created by filip on 19.10.2025.
//

#ifndef COURSEWORK_DIJKSTRA_H
#define COURSEWORK_DIJKSTRA_H
#include "Graph.h"
#pragma once


class Dijkstra {
public:
    static int shortestPath(const Graph& graph, int start, int end);
};


#endif //COURSEWORK_DIJKSTRA_H