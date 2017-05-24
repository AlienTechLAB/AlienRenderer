#pragma once
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/VertexShader.hpp"
#include "eVolution3D/UniformVariables.h"
#include "eVolution3D/Rasterizer.hpp"

using namespace eVolution3D;

class VertexProcessor
{
	//---------------------------------------------------------------------------------------------------------

	private:
	TargetBuffer<Color32>* FrameBuffer = nullptr;
	VertexShader*         VertShader = nullptr;
	UniformVariables      UniVariables;
	Vertex4               TransformedVertices[3];
	Rasterizer*           RasterizerStage = nullptr;

	//---------------------------------------------------------------------------------------------------------

	public: void SetTargetBuffer(TargetBuffer<Color32>* targetBuffer)
	{
		FrameBuffer = targetBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetVertexShader(VertexShader* vertexShader)
	{
		VertShader = vertexShader;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetMVP(const Matrix4x4& mvp)
	{
		UniVariables.MVP = mvp;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetRasterizer(Rasterizer* rasterizer)
	{
		RasterizerStage = rasterizer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void OnPointToPrecess(Vertex3* vertex)
	{
		VertexOperations(vertex, &TransformedVertices[0]);
		RasterizerStage->DrawPoint(&TransformedVertices[0], Color32::Green);
	}
	
	//---------------------------------------------------------------------------------------------------------

	public: void OnLineToPrecess(Vertex3* vertex1, Vertex3* vertex2)
	{
		VertexOperations(vertex1, &TransformedVertices[0]);
		VertexOperations(vertex2, &TransformedVertices[1]);
		RasterizerStage->DrawLine(&TransformedVertices[0], &TransformedVertices[1], Color32::Green);
	}
	
	//---------------------------------------------------------------------------------------------------------

	public: void OnTriangleToPrecess(Vertex3* vertex1, Vertex3* vertex2, Vertex3* vertex3)
	{
		VertexOperations(vertex1, &TransformedVertices[0]);
		VertexOperations(vertex2, &TransformedVertices[1]);
		VertexOperations(vertex3, &TransformedVertices[2]);

		RasterizerStage->DrawLine(&TransformedVertices[0], &TransformedVertices[1], Color32::Green);
		RasterizerStage->DrawLine(&TransformedVertices[1], &TransformedVertices[2], Color32::Green);
		RasterizerStage->DrawLine(&TransformedVertices[2], &TransformedVertices[0], Color32::Green);
	}

	//---------------------------------------------------------------------------------------------------------

	private: void VertexOperations(Vertex3* vetrexIn, Vertex4* vertexOut)
	{
		VertShader->ProcessVertex(vetrexIn, vertexOut, UniVariables);
		vertexOut->PerspectiveDivision();
		ViewportTransformation(vertexOut);
	}

	//---------------------------------------------------------------------------------------------------------

	private: void ViewportTransformation(Vertex4* vertex)
	{
		float halfWidth = (float)(FrameBuffer->GetWidth() >> 1);
		float halfHeight = (float)(FrameBuffer->GetHeight() >> 1);
		vertex->x = (vertex->x / FrameBuffer->GetAspectRatio()) * halfWidth + halfWidth;
		vertex->y = vertex->y * halfHeight + halfHeight;
	}

	//---------------------------------------------------------------------------------------------------------
};