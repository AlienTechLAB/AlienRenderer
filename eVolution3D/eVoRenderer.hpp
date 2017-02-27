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

		int w = FrameBuffer->GetWidth();
		int h = FrameBuffer->GetHeight();

		DrawLine(-100, 100, 100, 200, EVOCOLOR_GREEN);
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
		// Check if whole line is out of the screen
		if (x1 < 0 && x2 < 0)
			return;

		if (y1 < 0 && y2 < 0)
			return;

		int r = FrameBuffer->GetWidth() - 1;
		int t = FrameBuffer->GetHeight() - 1;

		if (x1 > r && x2 > r)
			return;

		if (y1 > t && y2 > t)
			return;



		if (x1 == x2)
		{
			if (y1 == y2)
			{
				// Draw dot - it's for sure on the screen
				FrameBuffer->PutPixel(x1, y1, color);
				return;
			}
			else
			{
				// It's vertical line
				     if (y1 < 0) y1 = 0;
				else if (y1 > t) y1 = t;

				     if (y2 < 0) y2 = 0;
				else if (y2 > t) y2 = t;

				DrawVerticalLine(x1, y1, y2, color);
				return;
			}
		}
		else if (y1 == y2)
		{
			// It's horizontal line
			     if (x1 < 0) x1 = 0;
			else if (x1 > r) x1 = r;

			     if (x2 < 0) x2 = 0;
			else if (x2 > r) x2 = r;

			DrawHorizontalLine(x1, x2, y1, color);
		}
		else
		{
			int dx = x2 - x1;
			int dy = y2 - y1;

			// Math formulas for clipping line:
			// y = (dy / dx) * (x - x1) + y1
			// x = (y - y1) * (dx / dy) + x1

			     if (x1 < 0) { y1 = (dy * -x1) / dx + y1; x1 = 0; }
			else if (x1 > r) { y1 = (dy * (r - x1)) / dx + y1; x1 = r; }

			     if (y1 < 0) { x1 = (-y1 * dx) / dy + x1; y1 = 0; }
			else if (y1 > t) { x1 = ((t - y1) * dx) / dy + x1; y1 = t; }

			     if (x2 < 0) { y2 = (dy * -x1) / dx + y1; x2 = 0; }
			else if (x2 > r) { y2 = (dy * (r - x1)) / dx + y1; x2 = r; }

			     if (y2 < 0) { x2 = (-y1 * dx) / dy + x1; y2 = 0; }
			else if (y2 > t) { x2 = ((t - y1) * dx) / dy + x1; y2 = t; }

			DrawDiagonalLine(x1, y1, x2, y2, color);
			return;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawVerticalLine(int x, int y1, int y2, int color)
	{
		int yMin, yMax;

		if (y2 > y1)
		{
			yMin = y1;
			yMax = y2;
		}
		else
		{
			yMin = y2;
			yMax = y1;
		}

		int screenWidth = FrameBuffer->GetWidth();
		int* address = (int*)FrameBuffer->GetPixelAddress(x, yMin);

		for (int y = yMin; y <= yMax; y++)
		{
			*address = color;
			address += screenWidth;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawHorizontalLine(int x1, int x2, int y, int color)
	{
		int xMin, xMax;

		if (x2 > x1)
		{
			xMin = x1;
			xMax = x2;
		}
		else
		{
			xMin = x2;
			xMax = x1;
		}

		int* address = (int*)FrameBuffer->GetPixelAddress(xMin, y);

		for (int y = xMin; y <= xMax; y++)
		{
			*address = color;
			address++;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawDiagonalLine(int x1, int y1, int x2, int y2, int color)
	{
		int* address = (int*)FrameBuffer->GetPixelAddress(x1, y1);
		int screenWidth = FrameBuffer->GetWidth();

		// Bresenham's line algorithm
		*address = color;

		int dx = x2 - x1;
		int dy = y2 - y1;
		int width = abs(dx);
		int height = abs(dy);

		int moveForwardAndUpX = (dx > 0) - (dx < 0);
		int moveForwardAndUpY = ((dy > 0) - (dy < 0)) * screenWidth;


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
			*address = color;
			threshold += height;

			if (width <= threshold)
			{
				threshold -= width;
				address += moveForwardAndUpX;
				address += moveForwardAndUpY;
			}
			else
			{
				address += moveForwardX;
				address += moveForwardY;
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------
};