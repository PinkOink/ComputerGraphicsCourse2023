#pragma once
#include <GameComponent.h>
#include <RenderContext.h>


class TrianglesNDC : public GameComponent
{
public:
  TrianglesNDC(RenderContext* context);

  virtual bool init();
  virtual bool update();
  virtual bool draw();

  virtual ~TrianglesNDC();

protected:
  RenderContext* mContext;
};

