#pragma once

#include <Memory/Array.h>
#include <Memory/String.h>

#include "bgfx/bgfx.h"

struct Texture
{

    Texture(String name, Int32 w, Int32 h, void* data);

    ~Texture();

    void use(UInt8 index, bgfx::UniformHandle handle) const;
    
    String textureName;
    Int32 textureWidth;
    Int32 textureHeight;
    struct TextureHandle* handle;

};
