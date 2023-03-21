#pragma once

#include <SimpleMath.h>


class RenderItem
{
public:
  virtual void setWorldMatrix(const DirectX::SimpleMath::Matrix& mat) = 0;

  virtual bool updateSubresources() = 0;
  virtual bool draw() = 0;

  virtual ~RenderItem() = default;
};