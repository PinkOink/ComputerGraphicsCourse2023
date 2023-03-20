#pragma once


class GameComponent
{
public:
  virtual bool init() = 0;
  virtual bool update(float deltaTime) = 0;
  virtual bool updateSubresources() = 0;
  virtual bool draw() = 0;

  virtual ~GameComponent() = default;
};

