#pragma once
#include "eVolution3D/eVoColor32.hpp"

class eVoFrameBuffer
{
	//---------------------------------------------------------------------------------------------------------

	public:
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetBufferSizeInBytes() = 0;
	virtual int GetBufferSizeInPixels() = 0;
	virtual eVoColor32* GetBuffer() = 0;
	virtual void OnResize(int newWidth, int newheight) = 0;
	virtual void PutPixel(int x, int y, eVoColor32 color) = 0;
	virtual eVoColor32* GetPixelAddress(int x, int y) = 0;

	//---------------------------------------------------------------------------------------------------------
};