#include <Rendering/Texture.h>
#include <bgfx/bgfx.h>

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

Texture::Texture(String name, Int32 w, Int32 h, void* data): textureName(std::move(name)), textureWidth(w), textureHeight(h)
{

    const auto* mem = bgfx::copy(data, w * h * sizeof(int));
    const auto texture = createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, 0, mem);

    this->handle = Allocator<TextureHandle>().construct(texture);
    
}

Texture::~Texture()
{
    Allocator<TextureHandle>().destroy(this->handle);
}

void Texture::use(UInt8 index, bgfx::UniformHandle handle) const {
    bgfx::setTexture(index, handle, this->handle->handle, 0);
}
