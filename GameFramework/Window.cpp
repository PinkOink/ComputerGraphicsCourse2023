#include "pch.h"
#include "Window.h"
#include "Game.h"


namespace
{
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Game::getGame()->msgProc(hwnd, msg, wParam, lParam);
	}
};


Window::Window()
{}

bool Window::init(HINSTANCE hInstance, const LPCWSTR& appName)
{
	if (mWindowHandle) 
	{
		MessageBox(0, L"Window has already been initialized", 0, 0);
		assert(false);
		return false;
	}

	LPCWSTR className = L"Window";

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = className;

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass failed", 0, 0);
		assert(false);
		return false;
	}

	// Don't care at this point
	auto screenWidth = 800;
	auto screenHeight = 800;

	RECT R = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mWindowHandle = CreateWindow(className, appName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!mWindowHandle)
	{
		MessageBox(0, L"CreateWindow failed", 0, 0);
		assert(false);
		return false;
	}

	ShowWindow(mWindowHandle, SW_SHOW);

	SetForegroundWindow(mWindowHandle);
	SetFocus(mWindowHandle);

	ShowCursor(true);

	return true;
}
