#pragma once

#include <bgfx/bgfx.h>

class IRenderable {

public:

    IRenderable() = default;

    virtual ~IRenderable() = default;

    virtual void render() = 0;

};
