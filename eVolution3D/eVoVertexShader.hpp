#pragma once
#include "eVoVector3.hpp"

class eVoVertexShader
{
	//---------------------------------------------------------------------------------------------------------

	public: virtual void ProcessVertex(eVoVector3* srcVertex, eVoVector3* dstVertex)
	{
		*dstVertex = *srcVertex;
	}

	//---------------------------------------------------------------------------------------------------------
};