#pragma once


class Game;


class Window final
{
public:
  Window();
  ~Window();

  Window(const Window& rhs) = delete;
  Window(Window&& rhs) = delete;
  Window& operator=(const Window& rhs) = delete;
  Window& operator=(Window&& rhs) = delete;

  bool init(Game* game, const char* appName, unsigned int width, unsigned int height);
  void processMessages();
  void rename(const char* newName);

  unsigned int getWidth() const;
  unsigned int getHeight() const;
  void* getWindowHandle() const;
  bool shouldQuit() const;

  void requestQuit();
};

