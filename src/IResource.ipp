#pragma once

class IResource {
public:
  virtual void release() = 0;
  virtual ~IResource() = default;
};
