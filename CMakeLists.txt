# CMakeList.txt : CMake project for MandelbrotCpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

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

# TODO: Add tests and install targets if needed.
