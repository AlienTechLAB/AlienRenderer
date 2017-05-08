#pragma once

struct eVoColor32
{
	//---------------------------------------------------------------------------------------------------------

	union
	{
		int Color32;

		struct
		{
			unsigned char B;
			unsigned char G;
			unsigned char R;
			unsigned char A;
		};
	};

	//---------------------------------------------------------------------------------------------------------

	eVoColor32(int color32)
	{
		Color32 = color32;
	}

	//---------------------------------------------------------------------------------------------------------

	eVoColor32(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static const eVoColor32 Black;
	public: static const eVoColor32 White;
	public: static const eVoColor32 Red;
	public: static const eVoColor32 Green;
	public: static const eVoColor32 Blue;
	public: static const eVoColor32 Yellow;

	//---------------------------------------------------------------------------------------------------------
};

const eVoColor32 eVoColor32::Black  = 0x00000000;
const eVoColor32 eVoColor32::White  = 0x00FFFFFF;
const eVoColor32 eVoColor32::Red    = 0x00FF0000;
const eVoColor32 eVoColor32::Green  = 0x0000FF00;
const eVoColor32 eVoColor32::Blue   = 0x000000FF;
const eVoColor32 eVoColor32::Yellow = 0x000000FF;