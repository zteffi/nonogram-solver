cmake_minimum_required(VERSION 3.10)

project(nonogram_solver)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nonogram_solver
	main.cpp
	solver.hpp
	solver.cpp)