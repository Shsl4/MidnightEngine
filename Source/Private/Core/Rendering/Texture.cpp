#include <Rendering/Texture.h>
#include <bgfx/bgfx.h>

#include "bimg/bimg.h"
#include "bx/allocator.h"
#include "bx/error.h"
#include "Exception/Exception.h"

#define HandleWrapper(name, type)                                \
                                                                 \
    struct name {                                                \
                                                                 \
        explicit name(type const& handle) : handle(handle) {     \
                                                                 \
        }                                                        \
                                                                 \
        ~name(){                                                 \
            bgfx::destroy(handle);                               \
        }                                                        \
                                                                 \
        type handle;                                             \
                                                                 \
    };

HandleWrapper(TextureHandle, bgfx::TextureHandle);

Texture::Texture(String name, UInt32 w, UInt32 h, void* data): textureName(std::move(name)), textureWidth(w), textureHeight(h)
{

    const auto* mem = bgfx::copy(data, w * h * sizeof(int));
    const auto texture = createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, 0, mem);

    this->handle = Allocator<TextureHandle>().construct(texture);
    
}

Texture::~Texture()
{
    Allocator<TextureHandle>().destroy(this->handle);
}

void Texture::use(UInt8 index, bgfx::UniformHandle uniform) const {
    bgfx::setTexture(index, uniform, this->handle->handle, 0);
}

CubeMap::CubeMap(String name, void* data, size_t size) {

    this->textureName = std::move(name);

    bx::DefaultAllocator allocator;
    bx::Error error;
    bimg::ImageContainer* container = bimg::imageParseDds(&allocator, data, static_cast<UInt32>(size * sizeof(UInt16)), &error);

    expectf(error.isOk(), "Cubemap loading error: {}", error.getMessage().getPtr());
    
    this->textureWidth = container->m_width;
    this->textureHeight = container->m_height;
    
    const auto* mem = bgfx::copy(container->m_data, container->m_size);

    const auto flags = 0
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
                | BGFX_SAMPLER_W_CLAMP;
    
    const auto texture = bgfx::createTextureCube(textureWidth, container->m_numMips > 1, container->m_numLayers,
        static_cast<bgfx::TextureFormat::Enum>(container->m_format), flags, mem);
    
    this->handle = Allocator<TextureHandle>().construct(texture);
    _aligned_free(container);
        
}

void CubeMap::use(UInt8 index, bgfx::UniformHandle uniform) const {

    uint32_t flags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
    bgfx::setTexture(index, uniform, this->handle->handle, flags);
    
}
