#pragma once

#include "MandelbrotPiece.h"
#include <memory>

class PieceFactory
{
public:
	virtual std::unique_ptr<MandelbrotPiece> create(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params)
	{
		return std::make_unique<MandelbrotPiece>(_pixels, _blockSize, _currentBlock, _params);
	}
};

class LoggingPieceFactory : public PieceFactory
{
public:
	virtual std::unique_ptr<MandelbrotPiece> create(std::vector<uint8_t>& _pixels, const size_t _blockSize, const size_t _currentBlock, ImageParams const& _params)
	{
		return std::make_unique<LoggingMandelbrotPiece>(_pixels, _blockSize, _currentBlock, _params);
	}
};