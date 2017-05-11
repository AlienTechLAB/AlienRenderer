#pragma once
#include "Constants.h"

#define DegToRad(a)((a * PI_NUMBER) / 180.0f)
#define RadToDeg(r)((r * 180.0f) / PI_NUMBER))

namespace eVolution3D
{
	// Fast inverse square root - Carmack's hack
	inline float InvSqrt(float value)
	{
		unsigned int i = *((unsigned int*)&value);
		i = 0x5f3759df - (i >> 1);

		float f = *((float*)&i);
		f = f * (1.5f - 0.5f * value * f * f);
		return f;
	}
}
