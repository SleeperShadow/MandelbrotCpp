#pragma once

#include <vector>
#include <future>

// size strings
static const char* s = "-s";
static const char* size = "-size";

// rect strings
static const char* r = "-r";
static const char* rect = "-rect";

// task strings
static const char* t = "-t";
static const char* tasks = "tasks";

// output strings
static const char* o = "-o";
static const char* output = "-output";

// quiet strings
static const char* q = "-q";
static const char* quiet = "-quiet";


/* Image parameters */

class ImageParams
{
public:
	std::string outputFile = "zad16.png";
	size_t width = 512;
	size_t height = 512;
	double xMin = -2.0;
	double xMax = 2.0;
	double yMin = -2.0;
	double yMax = 2.0;
	bool quietMode = false;
	std::vector<std::future<void>> runningTasks = {};
	size_t numThreads = 1;

	int iterations = 1000;

public:
	void initParameters(int argc, char** argv);
};