#pragma once
#include <windows.h>
#include "eVoFrameBuffer.h"

class eVoWindowsFrameBuffer : public eVoFrameBuffer
{
	//---------------------------------------------------------------------------------------------------------

	private:
	BITMAPINFO   BmpInfo;
	eVoColor32* Buffer = NULL;
	HDC          MemoryContext;
	HBITMAP      Bitmap;
	HGDIOBJ      OldBitmap;

	//---------------------------------------------------------------------------------------------------------

	public: ~eVoWindowsFrameBuffer()
	{
		Release();
	}

	//---------------------------------------------------------------------------------------------------------

	public: void OnResize(int newWidth, int newHeight) override
	{
		if (Bitmap != NULL)
		{
			SelectObject(MemoryContext, OldBitmap);
			DeleteObject(Bitmap);
			BmpInfo = GetBitmapInfo(newWidth, newHeight);
			Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, (void**)&Buffer, NULL, 0);
			OldBitmap = SelectObject(MemoryContext, Bitmap);
		}
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Init(HWND windowHandle)
	{
		RECT windowRect;
		GetClientRect(windowHandle, &windowRect);
		HDC deviceContext = GetDC(windowHandle);
		MemoryContext = CreateCompatibleDC(deviceContext);
		BmpInfo = GetBitmapInfo(windowRect.right, windowRect.bottom);
		Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, (void**)&Buffer, NULL, 0);
		OldBitmap = SelectObject(MemoryContext, Bitmap);
		ReleaseDC(windowHandle, deviceContext);
	}

	//---------------------------------------------------------------------------------------------------------

	private: BITMAPINFO GetBitmapInfo(int width, int height)
	{
		BITMAPINFO bmpInfo;

		bmpInfo.bmiHeader.biSize = sizeof(BmpInfo);
		bmpInfo.bmiHeader.biWidth = width;
		bmpInfo.bmiHeader.biHeight = height;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biSizeImage = ((width * (bmpInfo.bmiHeader.biBitCount / 8) + 3) & -4) * height;

		return bmpInfo;
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

	public: inline int GetWidth() override
	{
		return BmpInfo.bmiHeader.biWidth;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline int GetHeight() override
	{
		return BmpInfo.bmiHeader.biHeight;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline int GetBufferSizeInBytes() override
	{
		return BmpInfo.bmiHeader.biSizeImage;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline int GetBufferSizeInPixels() override
	{
		return BmpInfo.bmiHeader.biSizeImage >> 2;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline eVoColor32* GetBuffer() override
	{
		return Buffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: HDC GetMemoryContext()
	{
		return MemoryContext;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline void PutPixel(int x, int y, eVoColor32 color) override
	{
		eVoColor32* address = GetPixelAddress(x, y);
		*address = color;
	}

	//---------------------------------------------------------------------------------------------------------

	public: inline eVoColor32* GetPixelAddress(int x, int y) override
	{
		return &Buffer[(BmpInfo.bmiHeader.biWidth * y) + x];
	}

	//---------------------------------------------------------------------------------------------------------
};