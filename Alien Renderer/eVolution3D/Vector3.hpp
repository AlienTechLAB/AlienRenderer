#pragma once
#include <math.h> 
#include <exception>
#include "Math.hpp"
#include "Enums.h"

namespace eVolution3D
{
	struct Vector3
	{
		//---------------------------------------------------------------------------------------------------------

		public:
		float x, y, z;

		//---------------------------------------------------------------------------------------------------------

		public: Vector3()
		{
			x = y = z = 0;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3(float ax, float ay, float az)
		{
			x = ax;
			y = ay;
			z = az;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3 operator=(Vector3& vec3)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			return *this;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3 operator+(Vector3& vec3)
		{
			return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void operator+=(Vector3& vec3)
		{
			x += vec3.x;
			y += vec3.y;
			z += vec3.z;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3 operator-(Vector3& vec3)
		{
			return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void operator-=(Vector3& vec3)
		{
			x -= vec3.x;
			y -= vec3.y;
			z -= vec3.z;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3 operator*(float factor)
		{
			return Vector3(x * factor, y * factor, z * factor);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void operator*=(float factor)
		{
			x *= factor;
			y *= factor;
			z *= factor;
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vector3 operator/(float divisor)
		{
			if (divisor != 0)
				return Vector3(x / divisor, y / divisor, z / divisor);
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

		public: bool operator==(Vector3& vec3)
		{
			if (x != vec3.x) return false;
			else if (y != vec3.y) return false;
			else if (z != vec3.z) return false;
			else return true;
		}

		//---------------------------------------------------------------------------------------------------------

		public: bool operator!=(Vector3& vec3)
		{
			if (x != vec3.x) return true;
			else if (y != vec3.y) return true;
			else if (z != vec3.z) return true;
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
			}
			else
			{
				x = y = z = 0;
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
			x = y = z = 0;
		}

		//---------------------------------------------------------------------------------------------------------

		public: static Vector3 Zero()
		{
			return Vector3(0, 0, 0);
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetOne()
		{
			x = y = z = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: static Vector3 One()
		{
			return Vector3(1, 1, 1);
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
			return InvSqrt(x*x + y*y + z*z);
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
