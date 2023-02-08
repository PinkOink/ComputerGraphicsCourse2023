#pragma once


class Window final
{
public:
  Window();
  ~Window();

  Window(const Window& rhs) = delete;
  Window(Window&& rhs) = delete;
  Window& operator=(const Window& rhs) = delete;
  Window& operator=(Window&& rhs) = delete;

  bool init(const char* appName, unsigned int width, unsigned int height);
  void processMessages();

  unsigned int getWidth() const { return mWidth; }
  unsigned int getHeight() const { return mHeight; }
  void* getWindowHandle() const;
  bool shouldQuit() const;

protected:
  unsigned int mWidth = 0;
  unsigned int mHeight = 0;
};

