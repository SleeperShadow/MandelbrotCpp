#pragma once

#include <future>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
#include "algorithm"

#include "Params.h"
#include "MandelbrotPiece.h"
#include "TinyPngOut.hpp"
#include "PieceFactory.h"

template <bool loggingPiece>
class PieceManager;

inline int doWork(ImageParams& params, std::unique_ptr<PieceFactory> pieceFactory);

// no logging
template <>
class PieceManager<false>
{
public:
	PieceManager(ImageParams& _params) : params(_params) {}

	int run();

private:
	ImageParams& params;
};

inline int PieceManager<false>::run()
{
	return doWork(params, std::unique_ptr<PieceFactory>(new PieceFactory()));
}

template<>
class PieceManager<true>
{
public:
	PieceManager(ImageParams& _params) : params(_params) {}

	int run();

	~PieceManager();

private:
	ImageParams& params;
	std::chrono::high_resolution_clock ::time_point start;
};

inline int PieceManager<true>::run()
{
	start = std::chrono::high_resolution_clock::now();
	return doWork(params, std::unique_ptr<LoggingPieceFactory>(new LoggingPieceFactory()));
}

inline PieceManager<true>::~PieceManager()
{
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsedTime = end - start;
	auto totalTime = elapsedTime.count();

	std::cout << "Threads used in current run: < " + std::to_string(params.numThreads) + " >.\n";
	std::cout << "Total execution time for current run (millis): < " + std::to_string(totalTime) + " >.\n";
}

inline int doWork(ImageParams & params, std::unique_ptr<PieceFactory> pieceFactory)
{
	try {
		std::vector<uint8_t> pixels(params.height * params.width * 3);
		for (size_t threadId = 0; threadId < params.numThreads; ++threadId)
		{
			params.runningTasks.emplace_back(std::async(std::launch::async, [&params, threadId, &pixels, &pieceFactory]()
				{
					auto piece = pieceFactory->create(pixels, params,threadId);
					piece->run();
				}));
		}

		std::for_each(params.runningTasks.begin(), params.runningTasks.end(), [](std::future<void>& taskResult) {
			taskResult.wait();
			});


		std::ofstream out(params.outputFile, std::ios::binary);
		TinyPngOut pngout(static_cast<uint32_t>(params.width), static_cast<uint32_t>(params.height), out);
		pngout.write(pixels.data(), static_cast<size_t>(params.width * params.height));
		return EXIT_SUCCESS;

	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
}