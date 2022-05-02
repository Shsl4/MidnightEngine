#pragma once

#include <Core/EngineMacros.h>

/*!
 * An interface declaring an object as renderable
 */
class ENGINE_API Renderable {

public:

    Renderable() = default;

    virtual ~Renderable() = default;

    /*!
     * A function called to render the renderable.
     */
    virtual void render() = 0;

};
