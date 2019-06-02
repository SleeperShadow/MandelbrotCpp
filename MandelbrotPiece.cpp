#include "MandelbrotPiece.h"
#include <iostream>
#include "Params.h"
#include <chrono>
#include <string>
#include <cmath>

MandelbrotPiece::MandelbrotPiece(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params):
	pixels(_pixels),
	blockSize(_blockSize),
	currentBlock(_currentBlock),
	params(_params)
{
}

void MandelbrotPiece::run()
{
	size_t max_y = (currentBlock + 1) * blockSize;
	for (size_t y = currentBlock * blockSize; y < max_y; ++y)
	{
		for (size_t x = 0; x < params.width; ++x) {
			uint32_t pix = mandelbrot(x, y);
			pixels[(y * params.width * 3) + x * 3 + 0] = static_cast<uint8_t>(pix >> 16);
			pixels[(y * params.width * 3) + x * 3 + 1] = static_cast<uint8_t>(pix >> 8);
			pixels[(y * params.width * 3) + x * 3 + 2] = static_cast<uint8_t>(pix >> 0);
		}
	}
}

uint32_t MandelbrotPiece::mandelbrot(size_t x, size_t y) {
	double cr = params.xMin + (x + 0.5) / params.width * (params.xMax - params.xMin);
	double ci = params.yMax - (y + 0.5) / params.height * (params.yMax - params.yMin);
	double zr = 0;
	double zi = 0;
	int i;
	for (i = 0; i < params.iterations; i++) {
		if (zr * zr + zi * zi > 4)
			break;
		double temp = zr * zr - zi * zi + cr;
		zi = 2 * zr * zi + ci;
		zr = temp;
	}
	double j = (double)i / params.iterations;
	return static_cast<uint32_t>(std::pow(j, 0.4) * 255 + 0.5) << 16
		| static_cast<uint32_t>(std::pow(j, 0.7) * 255 + 0.5) << 8
		| static_cast<uint32_t>(std::pow(j, 0.3) * 255 + 0.5) << 0;
}

// init mutex with default value
std::mutex LoggingMandelbrotPiece::mutex{};

LoggingMandelbrotPiece::LoggingMandelbrotPiece(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params)
	: base(_pixels, _blockSize, _currentBlock, _params)
{
}

void LoggingMandelbrotPiece::run()
{
	onStart();
	auto start = std::chrono::high_resolution_clock::now();
	
	base::run();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsedTime = end - start;
	auto executionTime = elapsedTime.count();

	onEnd(executionTime);
}

void LoggingMandelbrotPiece::log(std::string const& message)
{
	lockType guard{ mutex };
	std::cout << message << std::endl;
}

void LoggingMandelbrotPiece::onStart()
{
	std::string&& startMsg = "Thread-< " + std::to_string(currentBlock) + " > started.";
	log(startMsg);
}

void LoggingMandelbrotPiece::onEnd(double executionTime)
{
	std::string&& endMsg = "Thread-< " + std::to_string(currentBlock) + " > stopped." + '\n' +
						   "Thread-< " + std::to_string(currentBlock) + " > execution time was(millis): < " + std::to_string(executionTime) + " >.";
	log(endMsg);
}
