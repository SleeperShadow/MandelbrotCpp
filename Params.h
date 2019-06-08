#pragma once

#include <vector>
#include <future>
#include <complex>
#include <memory>

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

// iterations strings
static const char* i = "-i";
static const char* iters = "-iters";

// tiled
static const char* tiled = "-tiled";

struct Tile
{
    bool canDraw = true;
    size_t tileX = 0;
    size_t tileY = 0;
};

/* Image parameters */

class ImageParams
{
public:
	std::string outputFile = "zad16.png";
	size_t width = 640;
	size_t height = 480;

	double xMin = -2.0;
	double xMax = 2.0;

	double yMin = -2.0;
	double yMax = 2.0;

	bool quietMode = false;
	bool isTiled = false;

	std::vector<std::future<void>> runningTasks = {};
	size_t numThreads = 1;

	int iterations = 1000;

	size_t tileSizeX;
	size_t tileSizeY;
private:
    mutable Tile currentTile;

    size_t maxTilesX;
    size_t maxTilesY;

    mutable std::mutex mutex;

public:
	void initParameters(int argc, char** argv);

	inline double getRealHeight() const
	{
		return yMax - yMin;
	}

	inline double getRealWidth() const
	{
		return xMax - xMin;
	}

	inline std::complex<double> transformPoint(size_t x, size_t y) const
	{
		return std::complex<double>{x / (double)width * getRealWidth() + xMin,
									y / (double)height * getRealHeight() + yMin };
	}

    inline const Tile getNextTile() const
    {
        std::lock_guard<std::mutex> lock{mutex};

        if( currentTile.tileX == maxTilesX)
        {
            currentTile.tileX = 0;
            ++currentTile.tileY;
        }

        currentTile.canDraw = currentTile.tileY < maxTilesY;
        auto result = currentTile;
        ++currentTile.tileX;

        return result;
    }
};