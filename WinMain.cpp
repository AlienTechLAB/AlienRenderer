#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>
#include "eVolution3D/eVoWindowsFrameBuffer.hpp"
#include "Application/Application.hpp"

//---------------------------------------------------------------------------------------------------------

HWND WindowHandle;

#define WIDTH 1024
#define HEIGHT 784

eVoWindowsFrameBuffer FrameBuffer;
Application Demo;

//---------------------------------------------------------------------------------------------------------

void OnWMPaint(HWND windowHandle)
{
	PAINTSTRUCT ps;
	BeginPaint(windowHandle, &ps);

	clock_t t1 = clock();
	Demo.UpdateFrameBuffer(&FrameBuffer);
	clock_t t2 = clock();

	HDC deviceContext = GetDC(windowHandle);
	BitBlt(deviceContext, 0, 0, FrameBuffer.GetWidth(), FrameBuffer.GetHeight(), FrameBuffer.GetMemoryContext(), 0, 0, SRCCOPY);

	SetBkMode(deviceContext, TRANSPARENT);
	SetTextColor(deviceContext, 0x00FFFFFF);
	long timeDifference = t2 - t1;
	float frameTime = ((float)timeDifference) / (float)CLOCKS_PER_SEC;
	int fps = 1.0f / frameTime;

	std::string text = "FPS: " + std::to_string(fps);
	TextOut(deviceContext, 0, 0, text.c_str(), text.length());
	ReleaseDC(windowHandle, deviceContext);

	EndPaint(windowHandle, &ps);
}

//---------------------------------------------------------------------------------------------------------

LONG WINAPI WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
			OnWMPaint(windowHandle);
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
			else
			{
				Demo.OnKeyPressed(wParam);
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
				Demo.Start();

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

					Demo.MainLoopUpdate();
					InvalidateRect(WindowHandle, NULL, false);
				}

				// Release
				Demo.Release();
				FrameBuffer.Release();
				DestroyWindow(WindowHandle);
			}
		}
	}
	
	return 0;
}

//---------------------------------------------------------------------------------------------------------