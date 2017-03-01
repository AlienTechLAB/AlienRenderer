#pragma once

struct eVoColorI32
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

	eVoColorI32(int color32)
	{
		Color32 = color32;
	}

	//---------------------------------------------------------------------------------------------------------

	eVoColorI32(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	//---------------------------------------------------------------------------------------------------------

	public: static const eVoColorI32 Black;
	public: static const eVoColorI32 White;
	public: static const eVoColorI32 Red;
	public: static const eVoColorI32 Green;
	public: static const eVoColorI32 Blue;
	public: static const eVoColorI32 Yellow;

	//---------------------------------------------------------------------------------------------------------
};

const eVoColorI32 eVoColorI32::Black  = 0x00000000;
const eVoColorI32 eVoColorI32::White  = 0x00FFFFFF;
const eVoColorI32 eVoColorI32::Red    = 0x00FF0000;
const eVoColorI32 eVoColorI32::Green  = 0x0000FF00;
const eVoColorI32 eVoColorI32::Blue   = 0x000000FF;
const eVoColorI32 eVoColorI32::Yellow = 0x000000FF;