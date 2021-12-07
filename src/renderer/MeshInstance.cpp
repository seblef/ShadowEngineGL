
#include "MeshInstance.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "GeometryData.h"
#include "Material.h"


MeshInstance::MeshInstance(const Mesh *model, const Matrix4& world, bool alwaysVisible) :
    Renderable(model->getGeometry()->getBBox(),world,alwaysVisible),
    _model(model)
{
    if(model->getMaterial()->isAnimated())		wakeUp();
}

MeshInstance::~MeshInstance()
{
    if(_model->getMaterial()->isAnimated())		sleep();
}


void MeshInstance::enqueue()
{
	MeshRenderer::getSingletonRef().addInstance(this);
}
