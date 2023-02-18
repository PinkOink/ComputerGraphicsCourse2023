#include "pch.h"
#include "Window.h"
#include "Game.h"


namespace
{
	struct WindowImpl final
	{
		Game* mGame = nullptr;

		HWND mWindowHandle = nullptr;
		HINSTANCE mInstanceHandle = nullptr;

		bool mQuitRequested = false;

		unsigned int mWidth = 0;
		unsigned int mHeight = 0;

		const LPCWSTR mClassNameWin = L"GameFrameworkWindow";
		const char* mClassNameChar = "GameFrameworkWindow";
	};

	static WindowImpl* gWindowImpl = nullptr;

	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		bool res = true;
		switch (msg)
		{
		case WM_SIZE:
			gWindowImpl->mWidth = LOWORD(lParam);
			gWindowImpl->mHeight = HIWORD(lParam);

			res = gWindowImpl->mGame->onResize(gWindowImpl->mWidth, gWindowImpl->mHeight);

			if (!res)
			{
				DestroyWindow(hwnd);
			}
			break;

		case WM_ENTERSIZEMOVE:
			gWindowImpl->mGame->pause();
			break;

		case WM_EXITSIZEMOVE:
			gWindowImpl->mGame->unpause();
			break;

		case WM_INPUT:
		{
			UINT dwSize = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == nullptr)
			{
				break;
			}

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
			}

			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				//	raw->data.keyboard.MakeCode,
				//	raw->data.keyboard.Flags,
				//	raw->data.keyboard.Reserved,
				//	raw->data.keyboard.ExtraInformation,
				//	raw->data.keyboard.Message,
				//	raw->data.keyboard.VKey);

				gWindowImpl->mGame->mInputDevice->OnKeyDown({
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.VKey,
					raw->data.keyboard.Message
					});
			} else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				gWindowImpl->mGame->mInputDevice->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
			}

			delete[] lpb;

			break;
		}

		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
};


Window::Window()
{}

Window::~Window()
{
	if (gWindowImpl)
	{
		if (gWindowImpl->mWindowHandle)
		{
			DestroyWindow(gWindowImpl->mWindowHandle);
		}
		if (gWindowImpl->mInstanceHandle)
		{
			UnregisterClassW(gWindowImpl->mClassNameWin, gWindowImpl->mInstanceHandle);
		}
	}
}

bool Window::init(Game* game, const char* appName, unsigned int width, unsigned int height)
{
	if (gWindowImpl)
	{
		MessageBox(0, L"Window has already been initialized", 0, 0);
		assert(false);
		return false;
	}
	if (game == nullptr)
	{
		MessageBox(0, L"Window didn't get Game instance", 0, 0);
		assert(false);
		return false;
	}

	gWindowImpl = new WindowImpl;
	gWindowImpl->mGame = game;
	gWindowImpl->mWidth = width;
	gWindowImpl->mHeight = height;

	gWindowImpl->mInstanceHandle = GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = gWindowImpl->mInstanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = gWindowImpl->mClassNameWin;

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass failed", 0, 0);
		assert(false);
		return false;
	}

	RECT R = { 0, 0, (LONG)gWindowImpl->mWidth, (LONG)gWindowImpl->mHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	gWindowImpl->mWindowHandle = CreateWindowA(gWindowImpl->mClassNameChar, appName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, gWindowImpl->mWidth, gWindowImpl->mHeight, 0, 0, gWindowImpl->mInstanceHandle, 0);
	if (!gWindowImpl->mWindowHandle)
	{
		MessageBox(0, L"CreateWindow failed", 0, 0);
		assert(false);
		return false;
	}

	ShowWindow(gWindowImpl->mWindowHandle, SW_SHOW);

	SetForegroundWindow(gWindowImpl->mWindowHandle);
	SetFocus(gWindowImpl->mWindowHandle);

	ShowCursor(true);

	return true;
}

void Window::processMessages()
{
	MSG message = {};

	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			gWindowImpl->mQuitRequested = true;
			break;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void Window::rename(const char* newName)
{
	SetWindowTextA(gWindowImpl->mWindowHandle, newName);
}

unsigned int Window::getWidth() const
{
	return gWindowImpl->mWidth;
}

unsigned int Window::getHeight() const
{
	return gWindowImpl->mHeight;
}

void* Window::getWindowHandle() const
{
	return &(gWindowImpl->mWindowHandle);
}

bool Window::shouldQuit() const
{
	return gWindowImpl->mQuitRequested;
}

void Window::requestQuit()
{
	gWindowImpl->mQuitRequested = true;
	DestroyWindow(gWindowImpl->mWindowHandle);
}
