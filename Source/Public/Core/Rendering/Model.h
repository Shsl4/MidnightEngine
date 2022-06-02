#pragma once

#include <Rendering/Mesh.h>
#include <Memory/SharedPointer.h>
#include <Math/Matrix4.h>
#include <Scene/LightComponent.h>

struct Model {
    Model(Array<SharedPointer<Mesh>> meshes, Array<SharedPointer<Texture>> textures, String name);

    FORCEINLINE size_t getMeshCount() const { return this->meshes.getSize(); }
    
    FORCEINLINE size_t getTextureCount() const { return this->boundTextures.getSize(); }
    
    NODISCARD Array<WeakPointer<Texture>> getTextures() const;
    
    FORCEINLINE Array<bgfx::ProgramHandle> getPrograms() const {

        return defaultHandles;
        
    }

    FORCEINLINE Array<Material> getMaterials() const {

        return defaultMaterials;
        
    }
        
    void render(UInt16 viewId,
                Vector4 const& view,
                Matrix4 const& transform,
                Array<WeakPointer<Texture>> const& textures,
                Array<Material> const& materials,
                Array<bgfx::ProgramHandle> const& programs,
                LightComponent* light) const;

    String modelName;

private:

    Array<bgfx::ProgramHandle> defaultHandles;
    Array<Material> defaultMaterials;
    Array<SharedPointer<Texture>> boundTextures;
    Array<SharedPointer<Mesh>> meshes;
    
};