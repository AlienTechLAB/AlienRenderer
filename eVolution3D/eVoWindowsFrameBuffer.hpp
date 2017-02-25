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

	public: void OnResize(int newWidth, int newHeight)
	{
		if (Bitmap != NULL)
		{
			SelectObject(MemoryContext, OldBitmap);
			DeleteObject(Bitmap);
			BmpInfo = GetBitmapInfo(newWidth, newHeight);
			Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, &Buffer, NULL, 0);
			OldBitmap = SelectObject(MemoryContext, Bitmap);
		}
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Init(int width, int height, HWND windowHandle)
	{
		HDC deviceContext = GetDC(windowHandle);
		MemoryContext = CreateCompatibleDC(deviceContext);
		BmpInfo = GetBitmapInfo(width, height);
		Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, &Buffer, NULL, 0);
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