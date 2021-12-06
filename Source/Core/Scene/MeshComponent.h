#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>

class MeshComponent : public SceneComponent, Renderable
{

    virtual void render() override;

    virtual bgfx::VertexLayout getVertexLayout() override;

};

