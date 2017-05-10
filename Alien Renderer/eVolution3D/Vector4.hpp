#pragma once
#include <math.h> 
#include <exception>
#include "Math.hpp"
#include "Enums.h"

struct Vector4
{
	//---------------------------------------------------------------------------------------------------------

	public:
	float x, y, z, w;

	//---------------------------------------------------------------------------------------------------------

	public: Vector4()
	{
		x = y = z = w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4(float ax, float ay, float az)
	{
		x = ax;
		y = ay;
		z = az;
		w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4 operator=(Vector4& vec4)
	{
		x = vec4.x;
		y = vec4.y;
		z = vec4.z;
		w = vec4.z;
		return *this;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4 operator+(Vector4& vec4)
	{
		return Vector4(x + vec4.x, y + vec4.y, z + vec4.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator+=(Vector4& vec4)
	{
		x += vec4.x;
		y += vec4.y;
		z += vec4.z;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4 operator-(Vector4& vec4)
	{
		return Vector4(x - vec4.x, y - vec4.y, z - vec4.z);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator-=(Vector4& vec4)
	{
		x -= vec4.x;
		y -= vec4.y;
		z -= vec4.z;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4 operator*(float factor)
	{
		return Vector4(x * factor, y * factor, z * factor);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void operator*=(float factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
	}

	//---------------------------------------------------------------------------------------------------------

	public: Vector4 operator/(float divisor)
	{
		if (divisor != 0)
			return Vector4(x / divisor, y / divisor, z / divisor);
		else
			throw std::exception("Division by 0!");
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
			throw std::exception("Division by 0!");
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator==(Vector4& vec4)
	{
		if (x != vec4.x) return false;
		else if (y != vec4.y) return false;
		else if (z != vec4.z) return false;
		else if (w != vec4.z) return false;
		else return true;
	}

	//---------------------------------------------------------------------------------------------------------

	public: bool operator!=(Vector4& vec4)
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
		float invLen = InvSqrt(x*x + y*y + z*z);

		if (invLen > 0)
		{
			x *= invLen;
			y *= invLen;
			z *= invLen;
			w = 0;
		}
		else
		{
			x = y = z = w = 0;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Rotate(float angle, Axis axis)
	{
		float c;

		switch (axis)
		{
			case Axis::X:
				c = y;
				y = y * cos(angle) - z * sin(angle);
				z = c * sin(angle) + z * cos(angle);
				break;

			case Axis::Y:
				c = x;
				x = z * sin(angle) + x * cos(angle);
				z = z * cos(angle) - c * sin(angle);
				break;

			case Axis::Z:
				c = x;
				x = y * sin(angle) + x * cos(angle);
				y = y * cos(angle) - c * sin(angle);
				break;

			default:
				throw std::exception("Unknown eVoAxis.");
		};
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetZero()
	{
		x = y = z = w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static Vector4 Zero()
	{
		return Vector4(0, 0, 0);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetOne()
	{
		x = y = z = 1;
		w = 0;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static Vector4 One()
	{
		return Vector4(1, 1, 1);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void SetCoords(float newX, float newY, float newZ, float newW)
	{
		x = newX;
		y = newY;
		z = newZ;
	}

	//---------------------------------------------------------------------------------------------------------

	public: float Magnitude()
	{
		return InvSqrt(x*x + y*y + z*z);
	}

	//---------------------------------------------------------------------------------------------------------
};
