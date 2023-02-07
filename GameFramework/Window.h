#pragma once


class Window
{
public:
  Window();

  bool init(HINSTANCE hInstance, const LPCWSTR& appName);

  HWND getWindowHandle() { return mWindowHandle; }

protected:
  HWND mWindowHandle = nullptr;
};

