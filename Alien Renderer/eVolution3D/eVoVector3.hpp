#pragma once
#include <math.h> 
#include "eVoException.hpp"
#include "eVoMath.hpp"
#include "eVoEnums.h"

struct eVoVector3
{
	//---------------------------------------------------------------------------------------------------------

	public:
	union
	{
		struct
		{
			float coords[3];
		};
	
		struct
		{
			float x, y, z;
		};
	};

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3()
	{
		x = y = z = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3(float ax, float ay, float az)
	{
		x = ax;
		y = ay;
		z = az;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator=(eVoVector3& vec3)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator+(eVoVector3& vec3)
	{
		return eVoVector3(x + vec3.x, y + vec3.y, z + vec3.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator+=(eVoVector3& vec3)
	{
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator-(eVoVector3& vec3)
	{
		return eVoVector3(x - vec3.x, y - vec3.y, z - vec3.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator-=(eVoVector3& vec3)
	{
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator*(float factor)
	{
		return eVoVector3(x * factor, y * factor, z * factor);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator*=(float factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator/(float divisor)
	{
		if (divisor != 0)
			return eVoVector3(x / divisor, y / divisor, z / divisor);
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
		}
		else
			throw eVoException("Division by 0!");
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator==(eVoVector3& vec3)
	{
		     if (x != vec3.x) return false;
		else if (y != vec3.y) return false;
		else if (z != vec3.z) return false;
		else return true;
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator!=(eVoVector3& vec3)
	{
		     if (x != vec3.x) return true;
		else if (y != vec3.y) return true;
		else if (z != vec3.z) return true;
		else return false;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Normalize()
	{
		float invLen = eVoInvSqrt(x*x + y*y + z*z);

		if (invLen > 0)
		{
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
		else
		{
			x = y = z = 0;
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
		x = y = z = 0;
	}
	
	//---------------------------------------------------------------------------------------------------------

	public: static eVoVector3 Zero()
	{
		return eVoVector3(0, 0, 0);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetOne()
	{
		x = y = z = 1;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static eVoVector3 One()
	{
		return eVoVector3(1, 1, 1);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetCoords(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}
	
	//---------------------------------------------------------------------------------------------------------

	public: float Magnitude()
	{
		return eVoInvSqrt(x*x + y*y + z*z);
	}

	//---------------------------------------------------------------------------------------------------------
};
