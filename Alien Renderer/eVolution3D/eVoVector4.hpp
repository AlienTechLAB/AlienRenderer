#pragma once
#include <math.h> 
#include "eVoException.hpp"
#include "eVoMath.hpp"
#include "eVoEnums.h"

struct eVoVector4
{
	//---------------------------------------------------------------------------------------------------------

	public:
	union
	{
		struct
		{
			float coords[4];
		};

		struct
		{
			float x, y, z, w;
		};
	};

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4()
	{
		x = y = z = w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4(float ax, float ay, float az, float aw)
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator=(eVoVector4 vec4)
	{
		x = vec4.x;
		y = vec4.y;
		z = vec4.z;
		w = vec4.z;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator+(eVoVector4 vec4)
	{
		return eVoVector4(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator+=(eVoVector4 vec4)
	{
		x += vec4.x;
		y += vec4.y;
		z += vec4.z;
		w += vec4.w;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator-(eVoVector4 vec4)
	{
		return eVoVector4(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator-=(eVoVector4 vec4)
	{
		x -= vec4.x;
		y -= vec4.y;
		z -= vec4.z;
		w -= vec4.w;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator*(float factor)
	{
		return eVoVector4(x * factor, y * factor, z * factor, w * factor);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator*=(float factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
		w *= factor;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator/(float divisor)
	{
		if (divisor != 0)
			return eVoVector4(x / divisor, y / divisor, z / divisor, w / divisor);
		else
			throw eVoException("Division by 0!");
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator/=(float divisor)
	{
		if (divisor != 0)
		{
			x /= divisor;
			y /= divisor;
			z /= divisor;
			w /= divisor;
		}
		else
			throw eVoException("Division by 0!");
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator==(eVoVector4 vec4)
	{
		if (x != vec4.x) return false;
		else if (y != vec4.y) return false;
		else if (z != vec4.z) return false;
		else if (w != vec4.z) return false;
		else return true;
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator!=(eVoVector4 vec4)
	{
		if (x != vec4.x) return true;
		else if (y != vec4.y) return true;
		else if (z != vec4.z) return true;
		else if (w != vec4.z) return true;
		else return false;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Normalize()
	{
		float invLen = eVoInvSqrt(x*x + y*y + z*z + w*w);

		if (invLen > 0)
		{
			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}
		else
		{
			x = y = z = w = 0;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Rotate(float angle, eVoAxis axis)
	{
		float c;

		switch (axis)
		{
		case eVoAxis::X:
			c = y;
			y = y * cos(angle) - z * sin(angle);
			z = c * sin(angle) + z * cos(angle);
			break;

		case eVoAxis::Y:
			c = x;
			x = z * sin(angle) + x * cos(angle);
			z = z * cos(angle) - c * sin(angle);
			break;

		case eVoAxis::Z:
			c = x;
			x = y * sin(angle) + x * cos(angle);
			y = y * cos(angle) - c * sin(angle);
			break;

		default:
			throw eVoException("Unknown eVoAxis.");
		};
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetZero()
	{
		x = y = z = w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static eVoVector4 Zero()
	{
		return eVoVector4(0, 0, 0, 0);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetOne()
	{
		x = y = z = w = 1;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static eVoVector4 One()
	{
		return eVoVector4(1, 1, 1, 1);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetCoords(float newX, float newY, float newZ, float newW)
	{
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	//---------------------------------------------------------------------------------------------------------

	public: float Magnitude()
	{
		return eVoInvSqrt(x*x + y*y + z*z + w*w);
	}

	//---------------------------------------------------------------------------------------------------------
};
