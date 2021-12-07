#pragma once

#include "ActorAnimInstance.h"
#include "Renderable.h"
#include "IUpdatable.h"
#include "../core/MatrixHierarchy.h"

class Actor;
class MeshInstance;

class ActorInstance : public Renderable, public IUpdatable
{
protected:

	Actor*						_model;
	ActorAnimInstance			_animation;
	MatrixTab					_pose;
	MatrixHierarchy				_worldMatrices;
	MatrixTab*					_boneMatrices;
	MeshInstance**				_nullNodesMeshes;

public:

	ActorInstance(Actor* model, const Matrix4& world, bool alwaysVisible);
	~ActorInstance();

	const Actor*			getModel() const					{ return _model; }

	MatrixTab&				getPose()							{ return _pose; }
	const MatrixTab&		getPose() const						{ return _pose; }
	const MatrixHierarchy&	getWorldMatrices() const			{ return _worldMatrices; }
	MatrixHierarchy&		getWorldMatrices()					{ return _worldMatrices; }
	const MatrixTab*		getBoneMatrices() const				{ return _boneMatrices; }

	void					enqueue();
	void					update(float time);

	void					playAnimation(const string& animName, bool loop=true, bool reverse=false);
	void					stopAnimation();
	void					applyOriginalPose();

	void					attachMeshToNullNode(MeshInstance *m, int nn);
	void					detachMeshToNullNode(int nn);

	void					attachMeshToNullNode(MeshInstance* m, const string& nullNodeName);
	void					detachMeshToNullNode(const string& nullNodeName);
};
