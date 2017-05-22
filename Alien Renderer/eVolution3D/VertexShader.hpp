#pragma once
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/Vertex4.hpp"
#include "eVolution3D/UniformVariables.h"

namespace eVolution3D
{
	class VertexShader
	{
		//---------------------------------------------------------------------------------------------------------

		public: virtual void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, UniformVariables& shaderIOData)
		{
			*vertexOut = *vetrexIn;
		}

		//---------------------------------------------------------------------------------------------------------
	};

	class VertexShaderMVP : public VertexShader
	{
		//---------------------------------------------------------------------------------------------------------

		public: void ProcessVertex(Vertex3* vetrexIn, Vertex4* vertexOut, UniformVariables& shaderIOData) override
		{
			*vertexOut = shaderIOData.MVP * (*vetrexIn);
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
