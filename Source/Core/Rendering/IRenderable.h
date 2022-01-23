#pragma once

#include <bgfx/bgfx.h>

/*!
 * An interface declaring an object as renderable
 */
class IRenderable {

public:

    IRenderable() = default;

    virtual ~IRenderable() = default;

    virtual void render() = 0;

};
