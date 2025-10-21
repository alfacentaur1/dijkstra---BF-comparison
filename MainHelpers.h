//
// Created by filip on 20.10.2025.
//

#ifndef PCC_SEMESTRALKA_MAINHELPERS_H
#define PCC_SEMESTRALKA_MAINHELPERS_H
#pragma once
#include "Graph.h"
#include <string>

Graph loadGraphFromFile(const std::string& filename);
void loadGraphFromStdin(Graph& g);
void loadGraphManual(Graph& g);
int getInputVertices();
void helperFunction();
int readInt(const std::string& prompt);
int readIntInRange(const std::string& prompt, int minValue, int maxValue);
Graph loadGraphFromArgs(int argc, char* argv[], int startIndex, int& outVertices);
#endif //PCC_SEMESTRALKA_MAINHELPERS_H