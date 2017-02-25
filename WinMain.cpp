#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <time.h>
#include "eVoWindowsFrameBuffer.hpp"

//---------------------------------------------------------------------------------------------------------

HWND WindowHandle;

#define WIDTH 1024
#define HEIGHT 784

int pixel = 0;

eVoWindowsFrameBuffer FrameBuffer;

//---------------------------------------------------------------------------------------------------------

void Render()
{
	int pixelsNo = FrameBuffer.GetBufferSizeInBytes() / 4;
	unsigned int* pBuffer = (unsigned int*)FrameBuffer.GetBuffer();

	for (int i = 0; i < pixelsNo; i++)
	{
		pBuffer[i] = pixel++;
	}
}

//---------------------------------------------------------------------------------------------------------

LONG WINAPI WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	clock_t t1, t2;
	long diff;
	float frameTime;
	float fps;
	HDC deviceContext;

	switch (uMsg)
	{
		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(windowHandle, &ps);

			t1 = clock();
			Render();
			t2 = clock();
			diff = t2 - t1;
			frameTime = ((float)diff) / (float)CLOCKS_PER_SEC;
			fps = 1.0f / frameTime;
			std::cout << fps << std::endl;

			deviceContext = GetDC(windowHandle);
			BitBlt(deviceContext, 0, 0, FrameBuffer.GetWidth(), FrameBuffer.GetHeight(), FrameBuffer.GetMemoryContext(), 0, 0, SRCCOPY);
			ReleaseDC(windowHandle, deviceContext);
			EndPaint(windowHandle, &ps);
			return 0;

		case WM_SIZE:
			PostMessage(windowHandle, WM_PAINT, 0, 0);
			return 0;

		case WM_CHAR:

			if (wParam == 27) // ESC key
			{
				PostQuitMessage(0);
				return 0;
			}

			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
	}

	return (LONG)DefWindowProc(windowHandle, uMsg, wParam, lParam);
}

//---------------------------------------------------------------------------------------------------------

void ShowConsole()
{
	AllocConsole();
	FILE* new_stdout;
	freopen_s(&new_stdout, "CONOUT$", "w", stdout);
}

//---------------------------------------------------------------------------------------------------------

bool CreateAndRegisterWndClass(LPCSTR className, WNDPROC windowProc)
{
	WNDCLASS wndClass;
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = windowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;

	if (RegisterClass(&wndClass) == false)
	{
		MessageBox(NULL, "CreateAndRegisterWndClass: RegisterClass failed.", "Error", MB_OK);
		return false;
	}
	else
	{
		return true;
	}
}

//---------------------------------------------------------------------------------------------------------

bool CreateAndShowWindow(int width, int height, const char* className, const char* title, int nCmdShow)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WindowHandle = CreateWindow(className, title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 100, 100, width, height, NULL, NULL, hInstance, NULL);

	if (WindowHandle == NULL)
	{
		MessageBox(NULL, "CreateAndShowWindow: CreateWindow failed.", "Error", MB_OK);
		return false;
	}
	else
	{
		ShowWindow(WindowHandle, nCmdShow);
		return true;
	}
}

//---------------------------------------------------------------------------------------------------------

bool DefinePixelFormat()
{
	PIXELFORMATDESCRIPTOR pixelFormat;
	memset(&pixelFormat, 0, sizeof(pixelFormat));
	pixelFormat.nSize = sizeof(pixelFormat);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW;
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cColorBits = 32;

	HDC deviceContext = GetDC(WindowHandle);
	int pf = ChoosePixelFormat(deviceContext, &pixelFormat);

	if (pf == 0)
	{
		MessageBox(NULL, "SetPixelFormat: ChoosePixelFormat failed.", "Error", MB_OK);
		return false;
	}
	else
	{
		if (SetPixelFormat(deviceContext, pf, &pixelFormat) == false)
		{
			MessageBox(NULL, "DefinePixelFormat: SetPixelFormat failed.", "Error", MB_OK);
			return false;
		}
		else
		{
			DescribePixelFormat(deviceContext, pf, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormat);
		}
	}

	ReleaseDC(WindowHandle, deviceContext);
	return true;
}

//---------------------------------------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	ShowConsole();

	if (CreateAndRegisterWndClass("WindowClass", (WNDPROC)WindowProc))
	{
		bool isWindowCreated = CreateAndShowWindow(WIDTH, HEIGHT, "WindowClass", "AlienRenderer", nCmdShow);

		if (isWindowCreated)
		{
			if (DefinePixelFormat())
			{
				FrameBuffer.Init(WIDTH, HEIGHT, WindowHandle);

				// Main loop
				while (true)
				{
					MSG msg;
					while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					if (msg.message == WM_QUIT)
						break;

					InvalidateRect(WindowHandle, NULL, false);
				}

				// Release
				FrameBuffer.Release();
				DestroyWindow(WindowHandle);
			}
		}
	}
	
	return 0;
}

//---------------------------------------------------------------------------------------------------------