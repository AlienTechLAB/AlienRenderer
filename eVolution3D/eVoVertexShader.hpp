#pragma once
#include "eVoVector3.hpp"
#include "eVoShaderIOData.h"

class eVoVertexShader
{
	//---------------------------------------------------------------------------------------------------------
	
	public: virtual void ProcessVertex(eVoVector3* vetrexIn, eVoVector3* vertexOut, eVoShaderIOData& shaderIOData)
	{
		*vertexOut = *vetrexIn;
	}

	//---------------------------------------------------------------------------------------------------------
};

class eVoVertexShaderMVP : public eVoVertexShader
{
	//---------------------------------------------------------------------------------------------------------

	public: void ProcessVertex(eVoVector3* vetrexIn, eVoVector3* vertexOut, eVoShaderIOData& shaderIOData) override
	{
		*vertexOut = shaderIOData.MVP * (*vetrexIn);
	}

	//---------------------------------------------------------------------------------------------------------
};
