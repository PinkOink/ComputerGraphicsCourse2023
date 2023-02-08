#pragma once


class GameComponent
{
public:
  virtual bool init() = 0;
  virtual bool update() = 0;
  virtual bool draw() = 0;

  virtual ~GameComponent() = default;
};

