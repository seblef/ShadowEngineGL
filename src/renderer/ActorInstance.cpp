#include "Actor.h"
#include "ActorInstance.h"
#include "ActorNullNode.h"
#include "ActorRenderer.h"
#include "MeshRenderer.h"
#include "ActorSkinnedRenderer.h"


ActorInstance::ActorInstance(Actor* model, const Matrix4& world, bool alwaysVisible) : Renderable(model->getBBox(),world,alwaysVisible),
    _model(model), _animation(model->getInitialPose().getCount()), _pose(model->getInitialPose()), _worldMatrices(model->getInitialPose()),
	_nullNodesMeshes(0)
{
	if(model->isSkinned())
		_boneMatrices=new MatrixTab(model->getBoneMatrices().getCount());
	else
		_boneMatrices=0;

	if(model->getNullNodeCount())
	{
		_nullNodesMeshes=new MeshInstance*[model->getNullNodeCount()];
		for(int i=0;i<model->getNullNodeCount();++i)
			_nullNodesMeshes[i]=0;
	}

	update(0);
}

ActorInstance::~ActorInstance()
{
    if(_boneMatrices)
        delete _boneMatrices;
}

void ActorInstance::enqueue()
{
	if(_boneMatrices)
		ActorSkinnedRenderer::getSingletonRef().addInstance(this);
	else
		ActorRenderer::getSingletonRef().addInstance(this);

	for(int i=0;i<_model->getNullNodeCount();++i)
	{
		if(_nullNodesMeshes[i])
			_nullNodesMeshes[i]->enqueue();
	}
}

void ActorInstance::update(float time)
{
	IUpdatable::update(time);

	_animation.update(time,_pose);
	_worldMatrices.update(_worldMatrix,_pose);

	if(_boneMatrices)
	{
		for(int b=0;b<_boneMatrices->getCount();++b)
			_boneMatrices->getItem(b)=_model->getBoneMatrices().getItem(b) * _worldMatrices.getItem(b);
	}

	for(int i=0;i<_model->getNullNodeCount();++i)
	{
		if(_nullNodesMeshes[i])
			_nullNodesMeshes[i]->setWorldMatrix(_worldMatrices.getItem(_model->getNullNode(i).getID()));
	}
}

void ActorInstance::playAnimation(const string& animName, bool loop, bool reverse)
{
    _animation.play(_model->getAnimation(animName),_pose,loop,reverse);
}

void ActorInstance::stopAnimation()
{
    _animation.stop();
}

void ActorInstance::applyOriginalPose()
{
    _pose=_model->getInitialPose();
}

void ActorInstance::attachMeshToNullNode(MeshInstance *m, int nn)
{
    assert(nn >= 0 && nn < _model->getNullNodeCount());
    _nullNodesMeshes[nn]=m;
}
void ActorInstance::detachMeshToNullNode(int nn)
{
    assert(nn >= 0 && nn < _model->getNullNodeCount());
    _nullNodesMeshes[nn]=0;
}

void ActorInstance::attachMeshToNullNode(MeshInstance* m, const string& nullNodeName)
{
    attachMeshToNullNode(m,_model->getNullNodeIndex(nullNodeName));
}

void ActorInstance::detachMeshToNullNode(const string& nullNodeName)
{
    detachMeshToNullNode(_model->getNullNodeIndex(nullNodeName));
}