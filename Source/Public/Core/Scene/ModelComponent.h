#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Rendering/LinearColor.h>
#include <Math/Matrix4.h>
#include <bgfx/bgfx.h>
#include <Math/Vector4.h>

#include <Memory/WeakPointer.h>
#include <Rendering/Mesh.h>
#include <Rendering/Texture.h>

#include "Rendering/Model.h"

/*!
 *  \brief A renderable component that represents a mesh.
 */
class ENGINE_API ModelComponent : public SceneComponent, public Renderable {

public:

    ModelComponent() = default;

    void update(float deltaTime) override;

    bool setModel(String const& name);
    bool setTexture(size_t index, String const& name);
    bool setShader(size_t index, bgfx::ProgramHandle handle);
    void setViewId(UInt16 id) {
        this->viewId = id;
    }
    void setMaterial(size_t index, Material const& material) const;
    
    NODISCARD Material& getMaterial(size_t index) const;
    NODISCARD WeakPointer<Model> getModel() const { return this->model; }

private:

    using Super = SceneComponent;

    void render() override;

    WeakPointer<Model> model = nullptr;

    Array<WeakPointer<Texture>> textures = Array<WeakPointer<Texture>>(0);
    Array<Material> materials = Array<Material>(0);
    Array<bgfx::ProgramHandle> handles = Array<bgfx::ProgramHandle>(0);

    UInt16 viewId = 0;
    
};

