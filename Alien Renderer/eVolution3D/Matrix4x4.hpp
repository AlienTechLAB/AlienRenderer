#pragma once
#include <string.h>
#include "Vector3.hpp"
#include "Vertex4.hpp"

namespace eVolution3D
{
	struct Matrix4x4
	{
		//---------------------------------------------------------------------------------------------------------

		public:
		float M[4][4]; // Notation: M[column][row]

		//---------------------------------------------------------------------------------------------------------

		public: Matrix4x4()
		{
			SetIdentity();
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetTranslation(Vector3& vector)
		{
			SetIdentity();
			M[3][0] = vector.x;
			M[3][1] = vector.y;
			M[3][2] = vector.z;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetScale(float scale)
		{
			SetZero();
			M[0][0] = scale;
			M[1][1] = scale;
			M[2][2] = scale;
			M[3][3] = 1;
		}
		
		//---------------------------------------------------------------------------------------------------------

		public: void SetFrustumProjection(float l, float r, float b, float t, float n, float f)
		{
			// Based on: http://www.songho.ca/opengl/gl_projectionmatrix.html

			SetZero();
			M[0][0] = (2.0f * n) / (r - l);
			M[2][0] = -((r + l) / (r - l));
			M[1][1] = (2.0f * n) / (t - b);
			M[2][1] = -((t + b) / (t - b));
			M[2][2] = f / (f - n);
			M[3][2] = -((n * f) / (f - n));
			M[2][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetOrthogonalProjection(float l, float r, float b, float t, float n, float f)
		{
			// Based on: http://www.songho.ca/opengl/gl_projectionmatrix.html

			SetZero();
			M[0][0] = 2.0f / (r - l);
			M[2][0] = -((r + l) / (r - l));
			M[1][1] = 2.0f / (t - b);
			M[2][1] = -((t + b) / (t - b));
			M[2][2] = f / (f - n);
			M[3][2] = -2.0f / (f - n);
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetEulerRotation(Vector3& eulerAngles)
		{
			float sx = sin(eulerAngles.x);
			float cx = cos(eulerAngles.x);
			float sy = sin(eulerAngles.y);
			float cy = cos(eulerAngles.y);
			float sz = sin(eulerAngles.z);
			float cz = cos(eulerAngles.z);

			SetZero();
			M[0][0] = cy*cz;
			M[1][0] = -cy*sz*cx + sy*sx;
			M[2][0] = cy*sz*sx + sy*cx;
			M[0][1] = sz;
			M[1][1] = cz*cx;
			M[2][1] = -cz*sx;
			M[0][2] = -sy*cz;
			M[1][2] = sy*sz*cx + cy*sx;
			M[2][2] = -sy*sz*sx + cy*cx;
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetXRotation(float xAngle)
		{
			float sinX = sin(xAngle);
			float cosX = cos(xAngle);

			SetZero();
			M[0][0] = 1;
			M[1][1] = cosX;
			M[1][1] = -sinX;
			M[2][1] = sinX;
			M[2][2] = cosX;
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetYRotation(float yAngle)
		{
			float sinY = sin(yAngle);
			float cosY = cos(yAngle);

			SetZero();
			M[0][0] = cosY;
			M[2][1] = sinY;
			M[1][1] = 1;
			M[0][2] = -sinY;
			M[2][2] = cosY;
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void SetZRotation(float zAngle)
		{
			float sinZ = sin(zAngle);
			float cosZ = cos(zAngle);

			SetZero();
			M[0][0] = cosZ;
			M[1][0] = -sinZ;
			M[0][1] = sinZ;
			M[1][1] = cosZ;
			M[2][2] = 1;
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: inline void SetIdentity()
		{
			SetZero();
			M[0][0] = 1;
			M[1][1] = 1;
			M[2][2] = 1;
			M[3][3] = 1;
		}

		//---------------------------------------------------------------------------------------------------------

		public: inline void SetZero()
		{
			memset(M, 0, sizeof(M));
		}

		//---------------------------------------------------------------------------------------------------------

		public: Vertex4 operator*(Vertex3& vector)
		{
			Vertex4 v;
			v.x = M[0][0] * vector.x + M[1][0] * vector.y + M[2][0] * vector.z + M[3][0];
			v.y = M[0][1] * vector.x + M[1][1] * vector.y + M[2][1] * vector.z + M[3][1];
			v.z = M[0][2] * vector.x + M[1][2] * vector.y + M[2][2] * vector.z + M[3][2];
			v.w = M[0][3] * vector.x + M[1][3] * vector.y + M[2][3] * vector.z + M[3][3];
			return v;
		}

		//---------------------------------------------------------------------------------------------------------

		public: void operator*=(Matrix4x4& m)
		{
			*this = (*this * m);
		}

		//---------------------------------------------------------------------------------------------------------

		public: Matrix4x4 operator*(Matrix4x4& m)
		{
			Matrix4x4 n;

			n.M[0][0] = M[0][0] * m.M[0][0] + M[1][0] * m.M[0][1] + M[2][0] * m.M[0][2] + M[3][0] * m.M[0][3];
			n.M[1][0] = M[0][0] * m.M[1][0] + M[1][0] * m.M[1][1] + M[2][0] * m.M[1][2] + M[3][0] * m.M[1][3];
			n.M[2][0] = M[0][0] * m.M[2][0] + M[1][0] * m.M[2][1] + M[2][0] * m.M[2][2] + M[3][0] * m.M[2][3];
			n.M[3][0] = M[0][0] * m.M[3][0] + M[1][0] * m.M[3][1] + M[2][0] * m.M[3][2] + M[3][0] * m.M[3][3];

			n.M[0][1] = M[0][1] * m.M[0][0] + M[1][1] * m.M[0][1] + M[2][1] * m.M[0][2] + M[3][1] * m.M[0][3];
			n.M[1][1] = M[0][1] * m.M[1][0] + M[1][1] * m.M[1][1] + M[2][1] * m.M[1][2] + M[3][1] * m.M[1][3];
			n.M[2][1] = M[0][1] * m.M[2][0] + M[1][1] * m.M[2][1] + M[2][1] * m.M[2][2] + M[3][1] * m.M[2][3];
			n.M[3][1] = M[0][1] * m.M[3][0] + M[1][1] * m.M[3][1] + M[2][1] * m.M[3][2] + M[3][1] * m.M[3][3];

			n.M[0][2] = M[0][2] * m.M[0][0] + M[1][2] * m.M[0][1] + M[2][2] * m.M[0][2] + M[3][2] * m.M[0][3];
			n.M[1][2] = M[0][2] * m.M[1][0] + M[1][2] * m.M[1][1] + M[2][2] * m.M[1][2] + M[3][2] * m.M[1][3];
			n.M[2][2] = M[0][2] * m.M[2][0] + M[1][2] * m.M[2][1] + M[2][2] * m.M[2][2] + M[3][2] * m.M[2][3];
			n.M[3][2] = M[0][2] * m.M[3][0] + M[1][2] * m.M[3][1] + M[2][2] * m.M[3][2] + M[3][2] * m.M[3][3];

			n.M[0][3] = M[0][3] * m.M[0][0] + M[1][3] * m.M[0][1] + M[2][3] * m.M[0][2] + M[3][3] * m.M[0][3];
			n.M[1][3] = M[0][3] * m.M[1][0] + M[1][3] * m.M[1][1] + M[2][3] * m.M[1][2] + M[3][3] * m.M[1][3];
			n.M[2][3] = M[0][3] * m.M[2][0] + M[1][3] * m.M[2][1] + M[2][3] * m.M[2][2] + M[3][3] * m.M[2][3];
			n.M[3][3] = M[0][3] * m.M[3][0] + M[1][3] * m.M[3][1] + M[2][3] * m.M[3][2] + M[3][3] * m.M[3][3];

			return n;
		}

		//---------------------------------------------------------------------------------------------------------
	};
}
