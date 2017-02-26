#pragma once
#include <math.h>
#include "eVolution3D/eVoFrameBuffer.h"
#include "eVolution3D/eVoConstants.h"

class eVoRenderer
{
	//---------------------------------------------------------------------------------------------------------

	private:
	eVoFrameBuffer* FrameBuffer = NULL;

	//---------------------------------------------------------------------------------------------------------

	public: void Render(eVoFrameBuffer* frameBuffer)
	{
		FrameBuffer = frameBuffer;
		ClearBufferWithColor(EVOCOLOR_BLACK);
		DrawLine(0, 0, frameBuffer->GetWidth(), frameBuffer->GetHeight(), EVOCOLOR_GREEN);
	}

	//---------------------------------------------------------------------------------------------------------

	private: void ClearBufferWithColor(int color)
	{
		int bufferSize = FrameBuffer->GetBufferSizeInPixels();
		int* buffer = (int*)FrameBuffer->GetBuffer();

		for (int i = 0; i < bufferSize; i++)
			buffer[i] = color;
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawLine(int x1, int y1, int x2, int y2, int color)
	{
		// Bresenham's line algorithm

		int dx = x2 - x1;
		int dy = y2 - y1;
		int width  = abs(dx);
		int height = abs(dy);

		int moveForwardAndUpX = (dx > 0) - (dx < 0);
		int moveForwardAndUpY = (dy > 0) - (dy < 0);


		int moveForwardX = moveForwardAndUpX;
		int moveForwardY = 0;

		if (width <= height)
		{
			moveForwardX = 0;
			moveForwardY = moveForwardAndUpY;
			width = abs(dy);
			height = abs(dx);
		}

		int threshold = width >> 1;

		for (int i = 0; i < width; i++)
		{
			FrameBuffer->PutPixel(x1, y1, color);
			threshold += height;

			if (width <= threshold)
			{
				threshold -= width;
				x1 += moveForwardAndUpX;
				y1 += moveForwardAndUpY;
			}
			else
			{
				x1 = x1 + moveForwardX;
				y1 = y1 + moveForwardY;
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------
};