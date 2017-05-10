#pragma once

namespace eVolution3D
{
	struct Color32
	{
		//---------------------------------------------------------------------------------------------------------

		union
		{
			unsigned int Color;

			struct
			{
				unsigned char B;
				unsigned char G;
				unsigned char R;
				unsigned char A;
			};
		};

		//---------------------------------------------------------------------------------------------------------

		Color32(int color32)
		{
			Color = color32;
		}

		//---------------------------------------------------------------------------------------------------------

		Color32(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}

		//---------------------------------------------------------------------------------------------------------

		public: static const Color32 Black;
		public: static const Color32 White;
		public: static const Color32 Red;
		public: static const Color32 Green;
		public: static const Color32 Blue;
		public: static const Color32 Yellow;

		//---------------------------------------------------------------------------------------------------------
	};

	const Color32 Color32::Black = 0x00000000;
	const Color32 Color32::White = 0x00FFFFFF;
	const Color32 Color32::Red = 0x00FF0000;
	const Color32 Color32::Green = 0x0000FF00;
	const Color32 Color32::Blue = 0x000000FF;
	const Color32 Color32::Yellow = 0x000000FF;
}