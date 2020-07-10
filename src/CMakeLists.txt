cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(SamplableSet)
set(CMAKE_CXX_STANDARD 17)

add_library(samplableset
    BinaryTree.cpp
    HashPropensity.cpp
    SamplableSet.cpp
)
