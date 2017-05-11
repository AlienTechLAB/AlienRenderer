#pragma once
#include <math.h>
#include <exception>
#include "eVolution3D/FrameBuffer.hpp"
#include "eVolution3D/Constants.h"
#include "eVolution3D/Color32.hpp"
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/VertexShader.hpp"
#include "eVolution3D/ShaderIOData.h"

namespace eVolution3D
{
	class Renderer
	{
		//---------------------------------------------------------------------------------------------------------

		private:
		FrameBuffer<Color32>* Frame = NULL;
		Vertex3* Vertices = NULL;
		int VerticesNo = 0;
		DrawingMode Mode = DrawingMode::POINT;
		VertexShader* VertShader = NULL;
		ShaderIOData ShaderIOData;

		//---------------------------------------------------------------------------------------------------------

		public: void SetTargetBuffer(FrameBuffer<Color32>* frameBuffer)
		{
			Frame = frameBuffer;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetVertices(Vertex3* vertices, int verticesNo)
		{
			Vertices = vertices;
			VerticesNo = verticesNo;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetDrawingMode(DrawingMode drawingMode)
		{
			Mode = drawingMode;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetMVPMatrix(const Matrix4x4& mvp)
		{
			ShaderIOData.MVP = mvp;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetVertexShader(VertexShader* vertexShader)
		{
			if (VertShader != NULL)
				delete VertShader;

			VertShader = vertexShader;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void ClearBufferWithColor(Color32 color)
		{
			int bufferSize = Frame->GetBufferSizeInPixels();
			Color32* buffer = (Color32*)Frame->GetBuffer();

			for (int i = 0; i < bufferSize; i++)
				buffer[i] = color;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void Render()
		{
			switch (Mode)
			{
				case DrawingMode::POINT: RenderPoints(); break;
				case DrawingMode::LINE: RenderLines(); break;
				case DrawingMode::LINE_STRIP: RenderLineStrip(); break;
				case DrawingMode::LINE_LOOP: RenderLineLoop(); break;
				case DrawingMode::WIRE_TRIANGLES: RenderWireTriangles(); break;
				case DrawingMode::TRIANGLE_STRIP: RenderWireTriangleStrip(); break;
				case DrawingMode::TRIANGLE_FAN: RenderWireTriangleFan(); break;
				default: throw std::exception("Unknown DrawingMode");
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderPoints()
		{
			Vertex4 vertex;

			for (int i = 0; i < VerticesNo; i++)
			{
				VertexOperations(&Vertices[i], &vertex);
				DrawPoint(&vertex, Color32::Green);
			}
		}

		 //---------------------------------------------------------------------------------------------------------

		private: void RenderLines()
		{
			Vertex4 vertices[2];
			int index = 0;
			int linesNo = VerticesNo >> 1;

			for (int i = 0; i < linesNo; i++)
			{
				VertexOperations(&Vertices[index++], &vertices[0]);
				VertexOperations(&Vertices[index++], &vertices[1]);
				DrawLine(&vertices[0], &vertices[1], Color32::Green);
			}
		}

		 //---------------------------------------------------------------------------------------------------------

		private: void RenderLineStrip()
		{
			Vertex4 vertices[2];
			int linesNo = VerticesNo - 1;

			if (linesNo > 0)
			{
				VertexOperations(&Vertices[0], &vertices[0]);

				for (int i = 1; i < VerticesNo; i++)
				{
					VertexOperations(&Vertices[i], &vertices[i]);
					DrawLine(&vertices[0], &vertices[1], Color32::Green);
					vertices[0] = vertices[1];
				}
			}
		}

		 //---------------------------------------------------------------------------------------------------------

		private: void RenderLineLoop()
		{
			Vertex4 vertices[2];

			if (VerticesNo > 2)
			{
				for (int i = 0; i < VerticesNo; i++)
				{
					VertexOperations(&Vertices[i], &vertices[0]);
					VertexOperations(&Vertices[(i + 1) % VerticesNo], &vertices[1]);
					DrawLine(&vertices[0], &vertices[1], Color32::Green);
				}
			}
		}

		 //---------------------------------------------------------------------------------------------------------

		private: void RenderWireTriangles()
		{
			Vertex4 vertices[3];
			int trianglesNo = VerticesNo / 3;

			if (trianglesNo > 0)
			{
				int index = 0;

				for (int i = 0; i < trianglesNo; i++)
				{
					VertexOperations(&Vertices[index++], &vertices[0]);
					VertexOperations(&Vertices[index++], &vertices[1]);
					VertexOperations(&Vertices[index++], &vertices[2]);
					DrawWireTriangle(&vertices[0], Color32::Green);
				}
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderWireTriangleStrip()
		{
			Vertex4 vertices[3];
			int index = 2;
			int trianglesNo = VerticesNo - 2;

			if (trianglesNo > 0)
			{
				VertexOperations(&Vertices[0], &vertices[0]);
				VertexOperations(&Vertices[1], &vertices[1]);

				for (int i = 0; i < trianglesNo; i++)
				{
					VertexOperations(&Vertices[index++], &vertices[2]);
					DrawWireTriangle(&vertices[0], Color32::Green);

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
			Vertex4 vertices[3];
			int index = 2;
			int trianglesNo = VerticesNo - 2;

			if (trianglesNo > 0)
			{
				VertexOperations(&Vertices[0], &vertices[0]);
				VertexOperations(&Vertices[1], &vertices[1]);

				for (int i = 0; i < trianglesNo; i++)
				{
					VertexOperations(&Vertices[index++], &vertices[2]);
					DrawWireTriangle(&vertices[0], Color32::Green);
					vertices[1] = vertices[2];
				}
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void VertexOperations(Vertex3* vetrexIn, Vertex4* vertexOut)
		{
			VertShader->ProcessVertex(vetrexIn, vertexOut, ShaderIOData);
			vertexOut->PerspectiveDivision();
			ViewportTransformation(vertexOut);
		}

		//---------------------------------------------------------------------------------------------------------

		private: void ViewportTransformation(Vertex4* vertex)
		{
			float halfWidth  = (float)(Frame->GetWidth() >> 1);
			float halfHeight = (float)(Frame->GetHeight() >> 1);
			vertex->x = (vertex->x / Frame->GetAspectRatio()) * halfWidth + halfWidth;
			vertex->y = vertex->y * halfHeight + halfHeight;
		}

		//---------------------------------------------------------------------------------------------------------

		private: void DrawPoint(Vertex4* vertices, Color32 color)
		{
			int x = (int)(vertices[0].x + 0.5f);
			int y = (int)(vertices[0].y + 0.5f);
			Color32* address = Frame->GetPixelAddress(x, y);
			*address = color;
		}

		//---------------------------------------------------------------------------------------------------------

		private: void DrawWireTriangle(Vertex4* vertices, Color32 color)
		{
			DrawLine(&vertices[0], &vertices[1], color);
			DrawLine(&vertices[1], &vertices[2], color);
			DrawLine(&vertices[2], &vertices[0], color);
		}

		//---------------------------------------------------------------------------------------------------------

		private: void DrawLine(Vertex4* vertex1, Vertex4* vertex2, Color32 color)
		{
			int x1 = (int)(vertex1->x + 0.5f);
			int y1 = (int)(vertex1->y + 0.5f);
			int x2 = (int)(vertex2->x + 0.5f);
			int y2 = (int)(vertex2->y + 0.5f);

			// Check if whole line is out of the screen
			if (x1 < 0 && x2 < 0)
				return;

			if (y1 < 0 && y2 < 0)
				return;

			int r = Frame->GetWidth() - 1;
			int t = Frame->GetHeight() - 1;

			if (x1 > r && x2 > r)
				return;

			if (y1 > t && y2 > t)
				return;



			if (x1 == x2)
			{
				if (y1 == y2)
				{
					// Draw dot - it's for sure on the screen
					Frame->PutPixel(x1, y1, color);
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

		private: void DrawVerticalLine(int x, int y1, int y2, Color32 color)
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

			int screenWidth = Frame->GetWidth();
			Color32* address = Frame->GetPixelAddress(x, yMin);

			for (int y = yMin; y <= yMax; y++)
			{
				*address = color;
				address += screenWidth;
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void DrawHorizontalLine(int x1, int x2, int y, Color32 color)
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

			Color32* address = Frame->GetPixelAddress(xMin, y);

			for (int y = xMin; y <= xMax; y++)
			{
				*address = color;
				address++;
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void DrawDiagonalLine(int x1, int y1, int x2, int y2, Color32 color)
		{
			Color32* address = Frame->GetPixelAddress(x1, y1);
			int screenWidth = Frame->GetWidth();

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
}
