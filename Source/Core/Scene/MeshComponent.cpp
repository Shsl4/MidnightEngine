#include "MeshComponent.h"
#include <bgfx/bgfx_utils.h>
void MeshComponent::render()
{

	Mesh* mesh = meshLoad("", false);

}

bgfx::VertexLayout MeshComponent::getVertexLayout()
{
	return bgfx::VertexLayout();
}
