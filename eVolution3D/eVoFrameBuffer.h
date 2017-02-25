#pragma once

class eVoFrameBuffer
{
	//---------------------------------------------------------------------------------------------------------

	public:
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetBufferSizeInBytes() = 0;
	virtual void* GetBuffer() = 0;
	virtual void OnResize(int newWidth, int newheight) = 0;

	//---------------------------------------------------------------------------------------------------------
};