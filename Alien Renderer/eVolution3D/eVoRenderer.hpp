#pragma once
#include <math.h>
#include "eVolution3D/eVoFrameBuffer.h"
#include "eVolution3D/eVoConstants.h"
#include "eVolution3D/eVoColor32.hpp"
#include "eVolution3D/eVoVertex3.hpp"
#include "eVolution3D/eVoVertexShader.hpp"
#include "eVoShaderIOData.h"

class eVoRenderer
{
	//---------------------------------------------------------------------------------------------------------

	private:
	eVoFrameBuffer<eVoColor32>* FrameBuffer = NULL;
	eVoVertex3* Vertices = NULL;
	int VerticesNo = 0;
	eVoDrawingMode DrawingMode = eVoDrawingMode::POINT;
	eVoVertexShader* VertexShader = NULL;
	eVoShaderIOData ShaderIOData;

	//---------------------------------------------------------------------------------------------------------

	public: void SetTargetBuffer(eVoFrameBuffer<eVoColor32>* frameBuffer)
	{
		FrameBuffer = frameBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetVertices(eVoVertex3* vertices, int verticesNo)
	{
		Vertices = vertices;
		VerticesNo = verticesNo;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetDrawingMode(eVoDrawingMode drawingMode)
	{
		DrawingMode = drawingMode;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetMVPMatrix(const eVoMatrix4x4& mvp)
	{
		ShaderIOData.MVP = mvp;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetVertexShader(eVoVertexShader* vertexShader)
	{
		if (VertexShader != NULL)
			delete VertexShader;

		VertexShader = vertexShader;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void ClearBufferWithColor(eVoColor32 color)
	{
		int bufferSize = FrameBuffer->GetBufferSizeInPixels();
		eVoColor32* buffer = (eVoColor32*)FrameBuffer->GetBuffer();

		for (int i = 0; i < bufferSize; i++)
			buffer[i] = color;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Render()
	{
		switch (DrawingMode)
		{
			case eVoDrawingMode::POINT: RenderPoints(); break;
			case eVoDrawingMode::LINE: RenderLines(); break;
			case eVoDrawingMode::LINE_STRIP: RenderLineStrip(); break;
			case eVoDrawingMode::LINE_LOOP: RenderLineLoop(); break;
			case eVoDrawingMode::WIRE_TRIANGLES: RenderWireTriangles(); break;
			case eVoDrawingMode::TRIANGLE_STRIP: RenderWireTriangleStrip(); break;
			case eVoDrawingMode::TRIANGLE_FAN: RenderWireTriangleFan(); break;
			default: throw eVoException("Unknown DrawingMode");
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderPoints()
	{
		eVoVertex4 vertex;

		for (int i = 0; i < VerticesNo; i ++)
		{
			VertexShader->ProcessVertex(&Vertices[i], &vertex, ShaderIOData);
			DrawPoint(&vertex, eVoColor32::Green);
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderLines()
	{
		eVoVertex4 vertices[2];
		int index = 0;
		int linesNo = VerticesNo >> 1;

		for (int i = 0; i < linesNo; i++)
		{
			VertexShader->ProcessVertex(&Vertices[index++], &vertices[0], ShaderIOData);
			VertexShader->ProcessVertex(&Vertices[index++], &vertices[1], ShaderIOData);
			DrawLine(&vertices[0], &vertices[1], eVoColor32::Green);
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderLineStrip()
	{
		eVoVertex4 vertices[2];
		int linesNo = VerticesNo - 1;

		if (linesNo > 0)
		{
			VertexShader->ProcessVertex(&Vertices[0], &vertices[0], ShaderIOData);

			for (int i = 1; i < VerticesNo; i++)
			{
				VertexShader->ProcessVertex(&Vertices[i], &vertices[i], ShaderIOData);
				DrawLine(&vertices[0], &vertices[1], eVoColor32::Green);
				vertices[0] = vertices[1];
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderLineLoop()
	{
		eVoVertex4 vertices[2];
		
		if (VerticesNo > 2)
		{
			for (int i = 0; i < VerticesNo; i++)
			{
				VertexShader->ProcessVertex(&Vertices[i], &vertices[0], ShaderIOData);
				VertexShader->ProcessVertex(&Vertices[(i + 1) % VerticesNo], &vertices[1], ShaderIOData);
				DrawLine(&vertices[0], &vertices[1], eVoColor32::Green);
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderWireTriangles()
	{
		eVoVertex4 vertices[3];
		int trianglesNo = VerticesNo / 3;

		if (trianglesNo > 0)
		{
			int index = 0;

			for (int i = 0; i < trianglesNo; i++)
			{
				VertexShader->ProcessVertex(&Vertices[index++], &vertices[0], ShaderIOData);
				VertexShader->ProcessVertex(&Vertices[index++], &vertices[1], ShaderIOData);
				VertexShader->ProcessVertex(&Vertices[index++], &vertices[2], ShaderIOData);
				DrawWireTriangle(&vertices[0], eVoColor32::Green);
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderWireTriangleStrip()
	{
		eVoVertex4 vertices[3];
		int index = 2;
		int trianglesNo = VerticesNo - 2;

		if (trianglesNo > 0)
		{
			VertexShader->ProcessVertex(&Vertices[0], &vertices[0], ShaderIOData);
			VertexShader->ProcessVertex(&Vertices[1], &vertices[1], ShaderIOData);

			for (int i = 0; i < trianglesNo; i++)
			{
				VertexShader->ProcessVertex(&Vertices[index++], &vertices[2], ShaderIOData);
				DrawWireTriangle(&vertices[0], eVoColor32::Green);

				if (i & 1)
					vertices[0] = vertices[2];
				else
					vertices[1] = vertices[2];
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void RenderWireTriangleFan()
	{
		eVoVertex4 vertices[3];
		int index = 2;
		int trianglesNo = VerticesNo - 2;

		if (trianglesNo > 0)
		{
			VertexShader->ProcessVertex(&Vertices[0], &vertices[0], ShaderIOData);
			VertexShader->ProcessVertex(&Vertices[1], &vertices[1], ShaderIOData);

			for (int i = 0; i < trianglesNo; i++)
			{
				VertexShader->ProcessVertex(&Vertices[index++], &vertices[2], ShaderIOData);
				DrawWireTriangle(&vertices[0], eVoColor32::Green);
				vertices[1] = vertices[2];
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawPoint(eVoVertex4* vertices, eVoColor32 color)
	{
		int x = vertices[0].x;
		int y = vertices[0].y;
		eVoColor32* address = FrameBuffer->GetPixelAddress(x, y);
		*address = color;
	}

	 //---------------------------------------------------------------------------------------------------------

	private: void DrawWireTriangle(eVoVertex4* vertices, eVoColor32 color)
	{
		DrawLine(&vertices[0], &vertices[1], color);
		DrawLine(&vertices[1], &vertices[2], color);
		DrawLine(&vertices[2], &vertices[0], color);
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawLine(eVoVertex4* vertex1, eVoVertex4* vertex2, eVoColor32 color)
	{
		int x1 = vertex1->x + 0.5f;
		int y1 = vertex1->y + 0.5f;
		int x2 = vertex2->x + 0.5f;
		int y2 = vertex2->y + 0.5f;

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

			if (x1 < 0 || x1 > r || y1 < 0 || y1 > t)
				return;


			     if (x2 < 0) { y2 = (dy * -x1) / dx + y1; x2 = 0; }
			else if (x2 > r) { y2 = (dy * (r - x1)) / dx + y1; x2 = r; }

			     if (y2 < 0) { x2 = (-y1 * dx) / dy + x1; y2 = 0; }
			else if (y2 > t) { x2 = ((t - y1) * dx) / dy + x1; y2 = t; }

			if (x2 < 0 || x2 > r || y2 < 0 || y2 > t)
				return;


			DrawDiagonalLine(x1, y1, x2, y2, color);
			return;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawVerticalLine(int x, int y1, int y2, eVoColor32 color)
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
		eVoColor32* address = FrameBuffer->GetPixelAddress(x, yMin);

		for (int y = yMin; y <= yMax; y++)
		{
			*address = color;
			address += screenWidth;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawHorizontalLine(int x1, int x2, int y, eVoColor32 color)
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

		eVoColor32* address = FrameBuffer->GetPixelAddress(xMin, y);

		for (int y = xMin; y <= xMax; y++)
		{
			*address = color;
			address++;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	private: void DrawDiagonalLine(int x1, int y1, int x2, int y2, eVoColor32 color)
	{
		eVoColor32* address = FrameBuffer->GetPixelAddress(x1, y1);
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