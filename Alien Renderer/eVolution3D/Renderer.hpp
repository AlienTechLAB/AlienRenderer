#pragma once
#include <math.h>
#include <exception>
#include "eVolution3D/FrameBuffer.hpp"
#include "eVolution3D/Constants.h"
#include "eVolution3D/Color32.hpp"
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/VertexShader.hpp"
#include "eVolution3D/UniformVariables.h"
#include "eVolution3D/InputAssembler.hpp"
#include "eVolution3D/VertexProcessor.hpp"
#include "eVolution3D/Rasterizer.hpp"

namespace eVolution3D
{
	class Renderer
	{
		//---------------------------------------------------------------------------------------------------------

		private:
		FrameBuffer<Color32>* TargetBuffer = nullptr;
		InputAssembler        InputAssemblerStage;
		VertexProcessor       VertexProcessorStage;
		Rasterizer            RasterizerStage;
		VertexShader*         VertShader = nullptr;

		//---------------------------------------------------------------------------------------------------------

		public: Renderer()
		{
			InputAssemblerStage.SetVertexProcessor(&VertexProcessorStage);
			VertexProcessorStage.SetRasterizer(&RasterizerStage);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetTargetBuffer(FrameBuffer<Color32>* targetBuffer)
		{
			TargetBuffer = targetBuffer;
			RasterizerStage.SetTargetBuffer(targetBuffer);
			VertexProcessorStage.SetTargetBuffer(targetBuffer);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetVertices(Vertex3* vertices, int verticesNo)
		{
			InputAssemblerStage.SetVertices(vertices, verticesNo);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetDrawingMode(DrawingMode drawingMode)
		{
			InputAssemblerStage.SetDrawingMode(drawingMode);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetMVPMatrix(const Matrix4x4& mvp)
		{
			VertexProcessorStage.SetMVP(mvp);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetVertexShader(VertexShader* vertexShader)
		{
			if (VertShader != NULL)
				delete VertShader;

			VertShader = vertexShader;
			VertexProcessorStage.SetVertexShader(VertShader);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void ClearBufferWithColor(Color32 color)
		{
			int bufferSize = TargetBuffer->GetBufferSizeInPixels();
			Color32* buffer = (Color32*)TargetBuffer->GetBuffer();

			for (int i = 0; i < bufferSize; i++)
				buffer[i] = color;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void Render()
		{
			InputAssemblerStage.Render();
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
