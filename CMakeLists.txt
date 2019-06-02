# CMakeList.txt : CMake project for MandelbrotCpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

project(MAndelbrotCpp)


find_package (Threads)

# Add source to this project's executable.
add_executable (MandelbrotCpp 
                "MandelbrotPng.cpp" 
				"TinyPngOut.cpp" 
				"TinyPngOut.hpp" 
				"Params.h" 
				"Params.cpp"
				"MandelbrotPiece.h" 
				"MandelbrotPiece.cpp"
				"PieceManager.h"
				"PieceFactory.h")

target_link_libraries (MandelbrotCpp ${CMAKE_THREAD_LIBS_INIT})

# TODO: Add tests and install targets if needed.
