cmake_minimum_required(VERSION 3.15)
project(pcc_tests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
        ../MainHelpers.cpp
        tests.cpp
        ../Graph.cpp
        ../Dijkstra.cpp
        ../BellmanFord.cpp
        catch.cpp
)

target_include_directories(tests PRIVATE ../)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
