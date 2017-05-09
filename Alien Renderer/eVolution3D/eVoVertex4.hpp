#pragma once
#include <math.h> 
#include "eVoException.hpp"
#include "eVoMath.hpp"
#include "eVoEnums.h"
#include "eVoVector4.hpp"

struct eVoVertex4
{
	//---------------------------------------------------------------------------------------------------------

	public:
	float x, y, z, w;

	//---------------------------------------------------------------------------------------------------------

	public: eVoVertex4()
	{
		x = y = z = 0;
		w = 1;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVertex4(float ax, float ay, float az)
	{
		x = ax;
		y = ay;
		z = az;
		w = 1;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVertex4 operator=(eVoVertex4& point)
	{
		x = point.x;
		y = point.y;
		z = point.z;
		w = point.w;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVertex4 operator=(eVoVertex3& point)
	{
		x = point.x;
		y = point.y;
		z = point.z;
		w = 1;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector4 operator-(eVoVertex4& point)
	{
		return eVoVector4(x - point.x, y - point.y, z - point.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator==(eVoVertex4& point)
	{
		if (x != point.x) return false;
		else if (y != point.y) return false;
		else if (z != point.z) return false;
		else if (w != point.z) return false;
		else return true;
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator!=(eVoVertex4& point)
	{
		if (x != point.x) return true;
		else if (y != point.y) return true;
		else if (z != point.z) return true;
		else if (w != point.z) return true;
		else return false;
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

	public: void SetCoords(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}
	
	//---------------------------------------------------------------------------------------------------------

	public: void PerspectiveDivision()
	{
		x /= w;
		y /= w;
		z /= w;
	}

	//---------------------------------------------------------------------------------------------------------
};