#pragma once
#include "eVolution3D/Color32.hpp"

namespace eVolution3D
{
	template <typename PixelType> class FrameBuffer
	{
		//---------------------------------------------------------------------------------------------------------

		public:
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual int GetBufferSizeInBytes() = 0;
		virtual int GetBufferSizeInPixels() = 0;
		virtual PixelType* GetBuffer() = 0;
		virtual void OnResize(int newWidth, int newheight) = 0;
		virtual void PutPixel(int x, int y, PixelType color) = 0;
		virtual PixelType* GetPixelAddress(int x, int y) = 0;

		//---------------------------------------------------------------------------------------------------------
	};
}
