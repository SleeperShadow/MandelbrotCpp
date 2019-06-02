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
			auto complexPoint = params.transformPoint(x, y);
			auto pix = escape(complexPoint);
			auto rgb = getRGB(pix);
			pixels[(y * params.width * 3) + x * 3 + 0] = std::get<0>(rgb);
			pixels[(y * params.width * 3) + x * 3 + 1] = std::get<1>(rgb);
			pixels[(y * params.width * 3) + x * 3 + 2] = std::get<2>(rgb);
		}
	}
}

uint32_t MandelbrotPiece::escape(std::complex<double> const& c) {
	std::complex<double> z(0, 0);
	int iter = 0;
	
	while (abs(z) < 2.0 && iter++ < params.iterations) {
		z = z * z + c * std::exp(-z);
	}
	return iter;
}

std::tuple<uint8_t, uint8_t, uint8_t> MandelbrotPiece::getRGB(uint32_t pix)
{
	uint32_t N = 256; // colors per element
	uint32_t N3 = N * N * N;
	// map n on the 0..1 interval (real numbers)
	double t = (double)pix / (double)params.iterations;
	// expand n on the 0 .. 256^3 interval (integers)
	pix = (int)(t * (double)N3);
	
	uint8_t b = pix / (N * N);
	uint8_t nn = pix - b * N * N;
	uint8_t r = nn / N;
	uint8_t g = nn - r * N;
	return std::tuple<uint8_t, uint8_t, uint8_t>(r, g, b);
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
