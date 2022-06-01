#pragma once

#include <Memory/Array.h>
#include <Memory/String.h>

#include "bgfx/bgfx.h"

struct ENGINE_API Texture
{

    Texture(String name, UInt32 w, UInt32 h, void* data);

    virtual ~Texture();

    virtual void use(UInt8 index, bgfx::UniformHandle uniform) const;
    
    String textureName = "";
    UInt32 textureWidth = 0;
    UInt32 textureHeight = 0;
    struct TextureHandle* handle = nullptr;

protected:

    Texture() = default;

};

struct ENGINE_API CubeMap : public Texture {

    CubeMap(String name, void* data, size_t size);

    void use(UInt8 index, bgfx::UniformHandle uniform) const override;

    
};
