## Makefile to compile a typical, simple C++14 project.
## Author       : Bart Groeneveld
## License      : GPLv3
## Latest update: Saturday, May 21st, 2016
## You are probably only interested in the first line of this Makefile.

# Libraries on which TARGET depends
LIBS = \
	-lpthread
# Name of the executable
TARGET = $(shell basename "$$PWD")
# Place to store all generated files
BUILD_DIR = build
# All source files. Header files are detected automatically
SRCS = MandelbrotPng.cpp MandelbrotPiece.cpp Params.cpp TinyPngOut.cpp
# Compiler. On non-gnu systems you may want to change this
CC = clang
# C++ version
EXTRAFLAGS = -std=c++14

SHELL = /bin/bash
DEPENDENCY_LIST = $(BUILD_DIR)/depend
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean

all: $(BUILD_DIR) $(DEPENDENCY_LIST) $(TARGET)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(EXTRAFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Every .o file will be matched here, and will be compiled from a .cpp file. Other dependencies are listed with '-include $(DEPENDENCY_LIST)'.
$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(EXTRAFLAGS) -c $< -o $@

# -MM outputs Makefile style dependencies of a source file
$(DEPENDENCY_LIST): $(SRCS) | $(BUILD_DIR)
	$(RM) $(DEPENDENCY_LIST)
	$(CC) $(EXTRAFLAGS) -MM $^ | awk '{print "$(BUILD_DIR)/./" $$0;}' >> $(DEPENDENCY_LIST)

# '-' prevents warning on first build or build after clean because dependencies files does not exist
-include $(DEPENDENCY_LIST)

# A note for maintainers of this Makefile: DO NOT REMOVE WHOLE BUILD_DIR! (would be a disaster if BUILD_DIR equals '.')
clean:
	$(RM) $(BUILD_DIR)/*.o *~ $(TARGET) $(DEPENDENCY_LIST)