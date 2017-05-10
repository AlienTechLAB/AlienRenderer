#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>
#include "eVolution3D/Color32.hpp"
#include "eVolution3D/eVoWindowsFrameBuffer.hpp"
#include "Application/Application.hpp"

//---------------------------------------------------------------------------------------------------------

#define WIDTH 1024
#define HEIGHT 768

HWND WindowHandle;
bool FullScreen = false;
RECT OldWindowMode;
bool QuitApplication = false;

eVoWindowsFrameBuffer<Color32> FrameBuffer;
Application Demo;

//---------------------------------------------------------------------------------------------------------

void OnWMPaint(HWND windowHandle)
{
	PAINTSTRUCT ps;
	BeginPaint(windowHandle, &ps);

	clock_t t1 = clock();
	Demo.UpdateFrameBuffer();
	clock_t t2 = clock();

	HDC deviceContext = GetDC(windowHandle);
	BitBlt(deviceContext, 0, 0, FrameBuffer.GetWidth(), FrameBuffer.GetHeight(), FrameBuffer.GetMemoryContext(), 0, 0, SRCCOPY);

	SetBkMode(deviceContext, TRANSPARENT);
	SetTextColor(deviceContext, 0x00FFFFFF);
	long timeDifference = t2 - t1;
	float frameTime = ((float)timeDifference) / (float)CLOCKS_PER_SEC;
	int fps = (int)(1.0f / frameTime);

	std::string text = "FPS: " + std::to_string(fps) + " Res: " + std::to_string(FrameBuffer.GetWidth()) + "x" + std::to_string(FrameBuffer.GetHeight());
	//TextOut(deviceContext, 0, 0, text.c_str(), (int)text.length());
	ReleaseDC(windowHandle, deviceContext);

	EndPaint(windowHandle, &ps);
}

//---------------------------------------------------------------------------------------------------------

void ToggleFullScreen()
{
	if (FullScreen == false)
	{
		GetWindowRect(WindowHandle, &OldWindowMode);
		SetWindowLongPtr(WindowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(WindowHandle, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLongPtr(WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		int width = OldWindowMode.right - OldWindowMode.left;
		int height = OldWindowMode.bottom - OldWindowMode.top;
		SetWindowPos(WindowHandle, HWND_TOP, OldWindowMode.left, OldWindowMode.top, width, height, SWP_SHOWWINDOW);
	}

	FullScreen = !FullScreen;
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
			RECT windowRect;
			GetClientRect(windowHandle, &windowRect);
			FrameBuffer.OnResize(windowRect.right, windowRect.bottom);
			PostMessage(windowHandle, WM_PAINT, 0, 0);
			return 0;

		case WM_CHAR:

			if (wParam == 27) // ESC key
			{
				QuitApplication = true;
				return 0;
			}
			else if (wParam == ' ')
			{
				ToggleFullScreen();
			}
			else
			{
				Demo.OnKeyPressed((char)wParam);
			}

			break;

		case WM_CLOSE:
			QuitApplication = true;
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
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

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
	WindowHandle = CreateWindow(className, title, WS_OVERLAPPEDWINDOW , 0, 0, width, height, NULL, NULL, hInstance, NULL);

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
				FrameBuffer.Init(WindowHandle);
				Demo.SetFrameBuffer(&FrameBuffer);
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

					Demo.MainLoopUpdate();
					InvalidateRect(WindowHandle, NULL, false);

					if (QuitApplication)
						break;
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