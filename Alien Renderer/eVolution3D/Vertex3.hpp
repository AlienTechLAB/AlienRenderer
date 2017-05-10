#pragma once
#include <math.h> 
#include "eVoException.hpp"
#include "eVoMath.hpp"
#include "eVoEnums.h"
#include "eVoVector3.hpp"

struct Vertex3
{
	//---------------------------------------------------------------------------------------------------------

	public:
	float x, y, z;

	//---------------------------------------------------------------------------------------------------------

	public: Vertex3()
	{
		x = y = z = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vertex3(float ax, float ay, float az)
	{
		x = ax;
		y = ay;
		z = az;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vertex3 operator=(Vertex3& point)
	{
		x = point.x;
		y = point.y;
		z = point.z;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: eVoVector3 operator-(Vertex3& point)
	{
		return eVoVector3(x - point.x, y - point.y, z - point.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator==(Vertex3& point)
	{
		if (x != point.x) return false;
		else if (y != point.y) return false;
		else if (z != point.z) return false;
		else return true;
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator!=(Vertex3& point)
	{
		if (x != point.x) return true;
		else if (y != point.y) return true;
		else if (z != point.z) return true;
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
};
