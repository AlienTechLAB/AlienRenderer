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

		DrawLine(700, 1200, 500, -100,EVOCOLOR_GREEN);
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

		int w = FrameBuffer->GetWidth();
		int h = FrameBuffer->GetHeight();

		if (x1 >= w && x2 >= w)
			return;

		if (y1 >= h && y2 >= h)
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
				if (y1 < 0)
					y1 = 0;
				else if (y1 >= h)
					y1 = h - 1;

				if (y2 < 0)
					y2 = 0;
				else if (y2 >= h)
					y2 = h - 1;
			}
		}
		else if (y1 == y2)
		{
			// It's horizontal line
			if (x1 < 0)
				x1 = 0;
			else if (x1 >= w)
				x1 = w - 1;

			if (x2 < 0)
				x2 = 0;
			else if (x2 >= w)
				x2 = w - 1;
		}
		else
		{
			// One vertex is out of the screen
			if (x1 < 0 || x1 >= w || y1 < 0 || y1 >= h || x2 < 0 || x2 >= w || y2 < 0 || y2 >= h)
			{
				int dx = x2 - x1;
				int dy = y2 - y1;

				if (x1 < 0)
				{
					int new_y1 = (dy * -x1) / dx + y1;

					// Is line invisible ?
					if (new_y1 < 0 || new_y1 >= h)
						return;
					else
					{
						x1 = 0;
						y1 = new_y1;
					}
				}
				else if (x1 >= w)
				{
					int new_y1 = (dy * (w - x1)) / dx + y1;

					// Is line invisible ?
					if (new_y1 < 0 || new_y1 >= h)
						return;
					else
					{
						x1 = w - 1;
						y1 = new_y1;
					}
				}



				if (x2 < 0)
				{
					int new_y2 = (dy * -x1) / dx + y1;

					// Is line invisible ?
					if (new_y2 < 0 || new_y2 >= h)
						return;
					else
					{
						x2 = 0;
						y2 = new_y2;
					}
				}
				else if (x2 >= w)
				{
					int new_y2 = (dy * (w - x1)) / dx + y1;

					// Is line invisible ?
					if (new_y2 < 0 || new_y2 >= h)
						return;
					else
					{
						x2 = w - 1;
						y2 = new_y2;
					}
				}



				if (y1 < 0)
				{
					int new_x1 = (-y1 * dx) / dy + x1;

					// Is line invisible ?
					if (new_x1 < 0 || new_x1 >= w)
						return;
					else
					{
						x1 = new_x1;
						y1 = 0;
					}
				}
				else if (y1 >= h)
				{
					int new_x1 = ((h - y1) * dx) / dy + x1;

					// Is line invisible ?
					if (new_x1 < 0 || new_x1 >= w)
						return;
					else
					{
						x1 = new_x1;
						y1 = h - 1;
					}
				}



				if (y2 < 0)
				{
					int new_x2 = (-y1 * dx) / dy + x1;

					// Is line invisible ?
					if (new_x2 < 0 || new_x2 >= w)
						return;
					else
					{
						x2 = new_x2;
						y2 = 0;
					}
				}
				else if (y2 >= h)
				{
					int new_x2 = ((h - y1) * dx) / dy + x1;

					// Is line invisible ?
					if (new_x2 < 0 || new_x2 >= w)
						return;
					else
					{
						x2 = new_x2;
						y2 = h - 1;
					}
				}
			}
		}

		DrawDiagonalLine(x1, y1, x2, y2, color);
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawDiagonalLine(int x1, int y1, int x2, int y2, int color)
	{
		// Bresenham's line algorithm
		FrameBuffer->PutPixel(x1, y1, color);

		int dx = x2 - x1;
		int dy = y2 - y1;
		int width = abs(dx);
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

	private: bool ClipLine(int& x1, int& y1, int& x2, int& y2)
	{
		if (IsVertexOnScreen(x1, y1) && IsVertexOnScreen(x2, y2))
			return true;

		if (IsLineForSureOutOfScreen(x1, y1, x2, y2))
			return false;

		if (PrependicularClipping(x1, y1, x2, y2))
			return true;



		return false;
	}

	//---------------------------------------------------------------------------------------------------------

	private: inline bool IsVertexOnScreen(int x, int y)
	{
		return ((x >= 0 && x <= FrameBuffer->GetWidth()) && (y >= 0 && y <= FrameBuffer->GetHeight()));
	}

	 //---------------------------------------------------------------------------------------------------------

	private: inline bool IsLineForSureOutOfScreen(int x1, int y1, int x2, int y2)
	{
		if (x1 < 0 && x2 < 0)
			return true;

		if (x1 > FrameBuffer->GetWidth() && x2 > FrameBuffer->GetWidth())
			return true;

		if (y1 < 0 && y2 < 0)
			return true;

		if (y1 > FrameBuffer->GetHeight() && y2 > FrameBuffer->GetHeight())
			return true;

		return false;
	}

	//---------------------------------------------------------------------------------------------------------

	private: inline bool PrependicularClipping(int& x1, int& y1, int& x2, int& y2)
	{


		return false;
	}

	//---------------------------------------------------------------------------------------------------------
};