#pragma once

#include "MandelbrotPiece.h"
#include <memory>

class PieceFactory
{
public:
	virtual std::unique_ptr<MandelbrotPiece> create(std::vector<uint8_t>& _pixels, ImageParams const& _params, size_t _threadId)
	{
		return std::unique_ptr<MandelbrotPiece>(new MandelbrotPiece(_pixels, _params, _threadId));
	}
};

class LoggingPieceFactory : public PieceFactory
{
public:
	virtual std::unique_ptr<MandelbrotPiece> create(std::vector<uint8_t>& _pixels, ImageParams const& _params, size_t _threadId)
	{
		return std::unique_ptr<LoggingMandelbrotPiece>(new LoggingMandelbrotPiece(_pixels, _params, _threadId));
	}
};