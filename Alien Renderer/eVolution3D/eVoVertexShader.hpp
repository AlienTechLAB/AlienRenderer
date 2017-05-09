#pragma once
#include "eVoVertex3.hpp"
#include "eVoVertex4.hpp"
#include "eVoShaderIOData.h"

class eVoVertexShader
{
	//---------------------------------------------------------------------------------------------------------
	
	public: virtual void ProcessVertex(eVoVertex3* vetrexIn, eVoVertex4* vertexOut, eVoShaderIOData& shaderIOData)
	{
		*vertexOut = *vetrexIn;
	}

	//---------------------------------------------------------------------------------------------------------
};

class eVoVertexShaderMVP : public eVoVertexShader
{
	//---------------------------------------------------------------------------------------------------------

	public: void ProcessVertex(eVoVertex3* vetrexIn, eVoVertex4* vertexOut, eVoShaderIOData& shaderIOData) override
	{
		*vertexOut = shaderIOData.MVP * (*vetrexIn);
	}

	//---------------------------------------------------------------------------------------------------------
};
