cmake_minimum_required(VERSION 3.15)
project(pcc-semestralka)

set(CMAKE_CXX_STANDARD 17)

# Main executable
add_executable(pcc-semestralka
        main.cpp
        Graph.cpp
        Dijkstra.cpp
        BellmanFord.cpp
        MainHelpers.h
        MainHelpers.cpp
)

target_include_directories(pcc-semestralka PRIVATE ${CMAKE_SOURCE_DIR})

# Include tests
add_subdirectory(tests)

# Enable testing (ctest integration)
enable_testing()
add_test(NAME GraphAlgorithmsTests COMMAND tests)

