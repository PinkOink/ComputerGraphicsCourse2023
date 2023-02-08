#pragma once


class Window final
{
public:
  Window();
  Window(const Window& rhs) = delete;
  Window(Window&& rhs) = delete;
  Window& operator=(const Window& rhs) = delete;
  Window& operator=(Window&& rhs) = delete;
  ~Window();

  bool init(const char* appName, unsigned int width, unsigned int height);
  void processMessages();

  void* getWindowHandle();
  bool shouldQuit();

protected:
  unsigned int mWidth = 0;
  unsigned int mHeight = 0;
};

