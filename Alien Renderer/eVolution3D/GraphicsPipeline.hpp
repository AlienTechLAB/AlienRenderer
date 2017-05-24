#pragma once
#include <math.h>
#include <exception>
#include "eVolution3D/TargetBuffer.hpp"
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
	class GraphicsPipeline
	{
		//---------------------------------------------------------------------------------------------------------

		private:
		TargetBuffer<Color32>* FrameBuffer = nullptr;
		InputAssembler        InputAssemblerStage;
		VertexProcessor       VertexProcessorStage;
		Rasterizer            RasterizerStage;
		VertexShader*         VertShader = nullptr;

		//---------------------------------------------------------------------------------------------------------

		public: GraphicsPipeline()
		{
			InputAssemblerStage.SetVertexProcessor(&VertexProcessorStage);
			VertexProcessorStage.SetRasterizer(&RasterizerStage);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetTargetBuffer(TargetBuffer<Color32>* targetBuffer)
		{
			FrameBuffer = targetBuffer;
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
			if (VertShader != nullptr)
				delete VertShader;

			VertShader = vertexShader;
			VertexProcessorStage.SetVertexShader(VertShader);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void Render()
		{
			InputAssemblerStage.Render();
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
