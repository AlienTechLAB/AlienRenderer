#pragma once
#include "Vertex3.hpp"
#include "Vertex4.hpp"
#include "ShaderIOData.h"

namespace eVolution3D
{
	class VertexShader
	{
		//---------------------------------------------------------------------------------------------------------

		public: virtual void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, ShaderIOData& shaderIOData)
		{
			*vertexOut = *vetrexIn;
		}

		//---------------------------------------------------------------------------------------------------------
	};

	class VertexShaderMVP : public VertexShader
	{
		//---------------------------------------------------------------------------------------------------------

		public: void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, ShaderIOData& shaderIOData) override
		{
			*vertexOut = shaderIOData.MVP * (*vetrexIn);
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
