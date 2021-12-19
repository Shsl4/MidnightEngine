#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>

class MeshComponent : public SceneComponent, public Renderable
{

    void render() override;

    bgfx::VertexLayout getVertexLayout() override;

};

