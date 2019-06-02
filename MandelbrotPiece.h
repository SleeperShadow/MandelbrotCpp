#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <tuple>
#include "Params.h"

using std::uint8_t;
using std::uint32_t;
using std::size_t;

class MandelbrotPiece
{
public:
	MandelbrotPiece(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params);

	virtual void run();

protected:

	uint32_t escape(std::complex<double> const& c);

	std::tuple<uint8_t, uint8_t, uint8_t> getRGB(uint32_t pix);

protected:

	std::vector<uint8_t>& pixels;
	size_t blockSize;
	size_t currentBlock;
	ImageParams const& params;

};

class LoggingMandelbrotPiece : public MandelbrotPiece
{
private:
	typedef MandelbrotPiece base;
	typedef std::mutex lock;
	typedef std::lock_guard<lock> lockType;

	static lock mutex;
public:
	LoggingMandelbrotPiece(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params);

	virtual void run() override;

private:
	static void log(std::string const& message);

	void onStart();
	void onEnd(double executionTime);
};