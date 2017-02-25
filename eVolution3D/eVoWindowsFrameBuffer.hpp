#pragma once
#include <windows.h>
#include "eVoFrameBuffer.h"

class eVoWindowsFrameBuffer : public eVoFrameBuffer
{
	//---------------------------------------------------------------------------------------------------------

	private:
	BITMAPINFO BmpInfo;
	void*      Buffer = NULL;
	HDC        MemoryContext;
	HBITMAP    Bitmap;
	HGDIOBJ    OldBitmap;

	//---------------------------------------------------------------------------------------------------------

	public: ~eVoWindowsFrameBuffer()
	{
		Release();
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Init(int width, int height, HWND windowHandle)
	{
		HDC deviceContext = GetDC(windowHandle);
		MemoryContext = CreateCompatibleDC(deviceContext);

		BmpInfo.bmiHeader.biSize = sizeof(BmpInfo);
		BmpInfo.bmiHeader.biWidth = width;
		BmpInfo.bmiHeader.biHeight = height;
		BmpInfo.bmiHeader.biPlanes = 1;
		BmpInfo.bmiHeader.biBitCount = 32;
		BmpInfo.bmiHeader.biCompression = BI_RGB;
		BmpInfo.bmiHeader.biSizeImage = ((width * (BmpInfo.bmiHeader.biBitCount / 8) + 3) & -4) * height;

		Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, &Buffer, NULL, 0);
		OldBitmap = SelectObject(MemoryContext, Bitmap);
		ReleaseDC(windowHandle, deviceContext);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Release()
	{
		if (OldBitmap != NULL)
		{
			SelectObject(MemoryContext, OldBitmap);
			OldBitmap = NULL;
		}

		if (Bitmap != NULL)
		{
			DeleteObject(Bitmap);
			Bitmap = NULL;
		}

		if (MemoryContext != NULL)
		{
			DeleteDC(MemoryContext);
			MemoryContext = NULL;
		}
	}

	//---------------------------------------------------------------------------------------------------------

	public: int GetWidth() override
	{
		return BmpInfo.bmiHeader.biWidth;
	}

	//---------------------------------------------------------------------------------------------------------

	public: int GetHeight() override
	{
		return BmpInfo.bmiHeader.biHeight;
	}

	//---------------------------------------------------------------------------------------------------------

	public: int GetBufferSizeInBytes() override
	{
		return BmpInfo.bmiHeader.biSizeImage;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void* GetBuffer() override
	{
		return Buffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: HDC GetMemoryContext()
	{
		return MemoryContext;
	}

	//---------------------------------------------------------------------------------------------------------
};