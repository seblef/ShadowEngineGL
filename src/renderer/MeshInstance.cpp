
#include "MeshInstance.h"
#include "MeshRenderer.h"

void MeshInstance::enqueue()
{
	MeshRenderer::getSingletonRef().addInstance(this);
}
