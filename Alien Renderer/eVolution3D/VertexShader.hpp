#pragma once
#include "Vertex3.hpp"
#include "Vertex4.hpp"
#include "eVoShaderIOData.h"

class VertexShader
{
	//---------------------------------------------------------------------------------------------------------
	
	public: virtual void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, eVoShaderIOData& shaderIOData)
	{
		*vertexOut = *vetrexIn;
	}

	//---------------------------------------------------------------------------------------------------------
};

class VertexShaderMVP : public VertexShader
{
	//---------------------------------------------------------------------------------------------------------

	public: void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, eVoShaderIOData& shaderIOData) override
	{
		*vertexOut = shaderIOData.MVP * (*vetrexIn);
	}

	//---------------------------------------------------------------------------------------------------------
};
