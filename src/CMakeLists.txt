cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(SamplableSet)
set(CMAKE_CXX_STANDARD 11)

add_library(samplableset
    BinaryTree.cpp
    HashPropensity.cpp
    SamplableSet.cpp
)

# Required to link SamplableSet in a shared library (e.g. other pybind module)
set_target_properties(samplableset PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
