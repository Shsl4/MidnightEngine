#pragma once

#include <bgfx/bgfx.h>

/*!
 * An interface declaring an object as renderable
 */
class IRenderable {

public:

    IRenderable() = default;

    virtual ~IRenderable() = default;

    /*!
     * A function called to render the renderable.
     */
    virtual void render() = 0;

};
