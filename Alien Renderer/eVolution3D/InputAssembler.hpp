#pragma once
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/Vertex4.hpp"
#include "eVolution3D/VertexProcessor.hpp"
#include "eVolution3D/Enums.h"

namespace eVolution3D
{
	class InputAssembler
	{
		//---------------------------------------------------------------------------------------------------------

		private:
		Vertex3*         Vertices = nullptr;
		int              VerticesNo = 0;
		DrawingMode      Mode = DrawingMode::NONE;
		VertexProcessor* VertProcessor;

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

		public: void SetVertexProcessor(VertexProcessor* vertexProcessor)
		{
			VertProcessor = vertexProcessor;
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
			for (int i = 0; i < VerticesNo; i++)
				VertProcessor->OnPointToPrecess(&Vertices[i]);
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderLines()
		{
			int index = 0;
			int linesNo = VerticesNo >> 1;

			for (int i = 0; i < linesNo; i++)
			{
				int index1 = i << 1;
				int index2 = (i << 1) | 1;
				VertProcessor->OnLineToPrecess(&Vertices[index1], &Vertices[index2]);
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderLineStrip()
		{
			int linesNo = VerticesNo - 1;

			if (linesNo > 0)
			{
				for (int i = 0; i < VerticesNo - 1; i++)
					VertProcessor->OnLineToPrecess(&Vertices[i], &Vertices[i + 1]);
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderLineLoop()
		{
			if (VerticesNo > 2)
			{
				for (int i = 0; i < VerticesNo; i++)
					VertProcessor->OnLineToPrecess(&Vertices[i], &Vertices[(i + 1) % VerticesNo]);
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderWireTriangles()
		{
			int trianglesNo = VerticesNo / 3;

			if (trianglesNo > 0)
			{
				for (int i = 0; i < trianglesNo; i++)
					VertProcessor->OnTriangleToPrecess(&Vertices[i * 3], &Vertices[i * 3 + 1], &Vertices[i * 3 + 2]);
			}
		}

		//---------------------------------------------------------------------------------------------------------

		private: void RenderWireTriangleStrip()
		{
			Vertex3* vertices[3];
			int index = 2;
			int trianglesNo = VerticesNo - 2;

			if (trianglesNo > 0)
			{
				vertices[0] = &Vertices[0];
				vertices[1] = &Vertices[1];

				for (int i = 0; i < trianglesNo; i++)
				{
					vertices[2] = &Vertices[index++];
					VertProcessor->OnTriangleToPrecess(vertices[0], vertices[1], vertices[2]);

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
			Vertex3* vertices[3];
			int index = 2;
			int trianglesNo = VerticesNo - 2;

			if (trianglesNo > 0)
			{
				vertices[0] = &Vertices[0];
				vertices[1] = &Vertices[1];

				for (int i = 0; i < trianglesNo; i++)
				{
					vertices[2] = &Vertices[index++];
					VertProcessor->OnTriangleToPrecess(vertices[0], vertices[1], vertices[2]);
					vertices[1] = vertices[2];
				}
			}
		}

		//---------------------------------------------------------------------------------------------------------
	};
}