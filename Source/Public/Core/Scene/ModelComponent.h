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

    void setModel(String const& name);
    
    NODISCARD WeakPointer<Model> getModel() const { return this->model; }

private:

    using Super = SceneComponent;

    void render(UInt64 state) override;

    WeakPointer<Model> model = nullptr;

    WeakPointer<Texture> texture = nullptr;
    
};

