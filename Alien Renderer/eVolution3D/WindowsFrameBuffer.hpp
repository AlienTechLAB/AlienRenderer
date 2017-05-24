#pragma once
#include <windows.h>
#include "TargetBuffer.hpp"

namespace eVolution3D
{
	template <typename PixelType> class WindowsFrameBuffer : public TargetBuffer<PixelType>
	{
		//---------------------------------------------------------------------------------------------------------

		private:
		BITMAPINFO BmpInfo;
		PixelType* Buffer = nullptr;
		HDC MemoryContext;
		HBITMAP Bitmap;
		HGDIOBJ OldBitmap;

		//---------------------------------------------------------------------------------------------------------

		public: ~WindowsFrameBuffer()
		{
			Release();
		}

		//---------------------------------------------------------------------------------------------------------

		public: void OnResize(int newWidth, int newHeight) override
		{
			if (Bitmap != nullptr)
			{
				SelectObject(MemoryContext, OldBitmap);
				DeleteObject(Bitmap);
				BmpInfo = GetBitmapInfo(newWidth, newHeight);
				Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, (void**)&Buffer, nullptr, 0);
				OldBitmap = SelectObject(MemoryContext, Bitmap);
				UpdateAspectRatio();
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
			Bitmap = CreateDIBSection(MemoryContext, &BmpInfo, DIB_RGB_COLORS, (void**)&Buffer, nullptr, 0);
			OldBitmap = SelectObject(MemoryContext, Bitmap);
			ReleaseDC(windowHandle, deviceContext);
			UpdateAspectRatio();
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
			if (OldBitmap != nullptr)
			{
				SelectObject(MemoryContext, OldBitmap);
				OldBitmap = nullptr;
			}

			if (Bitmap != nullptr)
			{
				DeleteObject(Bitmap);
				Bitmap = nullptr;
			}

			if (MemoryContext != nullptr)
			{
				DeleteDC(MemoryContext);
				MemoryContext = nullptr;
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

		public: inline Color32* GetBuffer() override
		{
			return Buffer;
		}

		//---------------------------------------------------------------------------------------------------------

		public: HDC GetMemoryContext()
		{
			return MemoryContext;
		}

		//---------------------------------------------------------------------------------------------------------

		public: inline void PutPixel(int x, int y, PixelType color) override
		{
			PixelType* address = GetPixelAddress(x, y);
			*address = color;
		}

		//---------------------------------------------------------------------------------------------------------

		public: inline PixelType* GetPixelAddress(int x, int y) override
		{
			return &Buffer[(BmpInfo.bmiHeader.biWidth * y) + x];
		}

		//---------------------------------------------------------------------------------------------------------

		public: void Clear(PixelType color)
		{
			int bufferSize = GetBufferSizeInPixels();
			PixelType* buffer = (PixelType*)GetBuffer();

			for (int i = 0; i < bufferSize; i++)
				buffer[i] = color;
		}

		//---------------------------------------------------------------------------------------------------------
	};
}