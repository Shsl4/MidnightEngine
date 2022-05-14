#pragma once

#include <Rendering/Mesh.h>
#include <Memory/SharedPointer.h>
#include <Math/Matrix4.h>

#include "Scene/BasicLightComponent.h"

struct Model {
    
    Model(Array<SharedPointer<Mesh>> meshes, String name, Material const& mat);
    
    FORCEINLINE size_t getMeshCount() const {
        return this->meshes.getSize();
    }

    FORCEINLINE void setShaderProgram(bgfx::ProgramHandle program) {
        this->handle = program;
    }

    FORCEINLINE void setMaterial(Material const& mat) {
        this->material = mat;
    }

    void render(UInt16 viewId, Vector4 const& view, Matrix4 const& transform, WeakPointer<BasicLightComponent> light) const;
    
    String modelName;

private:

    Material material;
    bgfx::ProgramHandle handle{};
    Array<SharedPointer<Mesh>> meshes;
    
};
